//  MSP430G2xx3 Demo - USCI_B0 I2C Master TX/RX multiple bytes from MSP430 Slave
//                     with a repeated start in between TX and RX operations.
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave, then a repeated start is generated followed by a
//  receive operation. This is the master code. This code demonstrates how to
//  implement an I2C repeated start with the USCI module using the USCI_B0 TX
//  interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.2MHz
//
//	***to be used with msp430x22x4_uscib0_i2c_13.c***
//
//                                /|\  /|\
//               MSP430F24x      10k  10k     MSP430G2xx3
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P3.1/UCB0SDA|<-|---+->|P3.1/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P3.2/UCB0SCL|<-+----->|P3.2/UCB0SCL     |
//            |                 |         |                 |
//
//  D. Dang
//  Texas Instruments Inc.
//  February 2011
//  Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "msp430g2553.h"
#include "lcdchar.h"
#include "bme280.h"


#define GAUGETGT 0x55
#define TMP100TGT 0x49
#define BME280TGT 0x76

unsigned char num_bytes_tx = 2;                         // How many bytes?

char stMessage[] = "This string is sent from msp430 sensor.";

int RXByteCtr, RPT_Flag = 0x0;                // enables repeated start when 1
unsigned char RxBuffer[128] = { 0 };
unsigned char *PTxData;                     // Pointer to TX data
unsigned char *PRxData;                     // Pointer to RX data
unsigned char TXByteCtr, RX = 0;
unsigned char MSData[] = { 0x3e, 0x02, 0x00 };
int lastByte;
double temperature, pressure, humidity;

int sendString = 0;
int tickCount = 0;
volatile unsigned int totalCount = 0;

void Setup_TX(unsigned char);
void Setup_RX(unsigned char);
void Transmit(unsigned char *datatoSend, unsigned char numBytestoSend);
void Receive(int);
void I2CWriteBlock(unsigned char, unsigned char[], int);
void I2CReadBlock(unsigned char tgtAddress, unsigned char *dataRead, int numBytestoRead);
void initTMP100();
void readTMP100();
void initUART();
void SendSerialString (char *);
void I2CReadBlock(unsigned char tgtAddress, unsigned char reg, unsigned char *dataRead, int numBytestoRead);


void main(void) {

	char sbuf[20] = { 0 };
	Adafruit_BME280 sensor;
	unsigned char dataBuf[10];
	char * pchar;
	int status;

	Lcd dispObj(0x27);

	WDTCTL = WDT_ADLY_1000;                   // WDT 250ms, ACLK, interval timer
	IE1 |= WDTIE;                             // Enable WDT interrupt
	__bic_SR_register(GIE);				//Disable

	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2 |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1DIR |= 0x01;                            // Set P1.0 to output direction

	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;

	initUART();
	__bis_SR_register(GIE);   //Enable
	//initTMP100();
	dispObj.Setup4bit();
	dispObj.WriteString(0, 0, "Temp Pres Humidity");

//	I2CReadBlock(BME280TGT, BME280_REGISTER_CHIPID, dataBuf, 1);
//	I2CReadBlock(BME280TGT, 0xE2, dataBuf, 8);

	bool retValue = sensor.begin();
	if (!retValue) {
		dispObj.WriteString(1, 0, "BME280 not found");
	}

	dispObj.DelayMsec(500);
	while (1) {

		if (sendString == 1) {
			totalCount += 1;
			temperature = (sensor.readTemperature() * 1.8 + 32) * 10.0;
			//Conversion 1 to 00.0002953
			pressure = sensor.readPressure() * 0.02953;

			humidity = sensor.readHumidity() * 10;
			sprintf(sbuf, "T%d P%d H%d", (int) temperature, (int) pressure,
					(int) humidity);
			dispObj.WriteString(2, 0, sbuf);
			SendSerialString(sbuf);

			SendSerialString(stMessage);
			sprintf(sbuf, "%6u", totalCount);
			dispObj.WriteString(3, 0, sbuf);
			sendString = 0;
		}
	}
}


/*
void origWriteByteReadBlock() {

	 //Transmit process
	  Setup_TX();
	  RPT_Flag = 1;
	  MSData[0] = BME280_REGISTER_CHIPID;
	  Transmit();
	  while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

	  //Receive process
	  Setup_RX();
	  Receive();
	  while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	  }
	}
}
*/


//------------------------------------------------------------------------------
// Transmit a string over the UART
//------------------------------------------------------------------------------
void txUART_string(char *string) {

	while (*string) {
		while (!(IFG2 & UCA0TXIFG))
			; // USCI_A0 TX buffer ready?
		UCA0TXBUF = *string++;
	}
}


void SendSerialString(char * lclMessage) {
	char sbuf[20];

	txUART_string(lclMessage);
	sprintf(sbuf, "  Count %6u\r\n", totalCount);
	txUART_string(sbuf);
}


//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
	__bic_SR_register(GIE);				//Disable

	if (RX == 1) {                              // Master Recieve?
		RXByteCtr--;                              // Decrement RX byte counter
		if (RXByteCtr >= 0) {
			*PRxData++ = UCB0RXBUF;           // Move RX data to address PRxData
		}
		if (RXByteCtr) {
		//	*PRxData++ = UCB0RXBUF;           // Move RX data to address PRxData
			lastByte = UCB0RXBUF;
		} else {
			if (RPT_Flag == 0)
				UCB0CTL1 |= UCTXSTP;        // No Repeated Start: stop condition
			if (RPT_Flag == 1) {                // if Repeated Start: do nothing
				RPT_Flag = 0;
			}
			__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
		}
	} else {                                     // Master Transmit
		if (TXByteCtr)                        // Check TX byte counter
		{
			UCB0TXBUF = *PTxData++;                   // Load TX buffer
			TXByteCtr--;                            // Decrement TX byte counter
		} else {
			if (RPT_Flag == 1) {
				RPT_Flag = 0;
				PTxData = &MSData[0];                  // TX array start address
				TXByteCtr = num_bytes_tx;                // Load TX byte counter
				__bic_SR_register_on_exit(CPUOFF);
			} else {
				UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
				IFG2 &= ~UCB0TXIFG;                 // Clear USCI_B0 TX int flag
				__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
			}
		}
	}
	__bis_SR_register(GIE);   //Enable
}


// These are the serial interrupts
//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	while (!(IFG2 & UCA0TXIFG))
		;                // USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
	P1OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
	tickCount++;
	if (tickCount > 1) {
		tickCount = 0;
		sendString = 1;
	}
}


void Setup_TX(unsigned char tgtAddress) {
	_DINT();
	RX = 0;
	IE2 &= ~UCB0RXIE;
	while (UCB0CTL1 & UCTXSTP)
		;               // Ensure stop condition got sent// Disable RX interrupt
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = tgtAddress;                         // Slave Address
	UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	IE2 |= UCB0TXIE;                          // Enable TX interrupt
}


void Setup_RX(unsigned char tgtAddress) {
	_DINT();
	RX = 1;
	IE2 &= ~UCB0TXIE;
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = tgtAddress;                         // Slave Address
	UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	IE2 |= UCB0RXIE;                          // Enable RX interrupt
}

void Transmit(unsigned char *datatoSend, unsigned char numBytestoSend) {
	PTxData = datatoSend;                      // TX array start address
	TXByteCtr = numBytestoSend - 1;                  // Load TX byte counter
	while (UCB0CTL1 & UCTXSTP)
		;             // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
	while (UCB0CTL1 & UCTXSTP)
		;             // Ensure stop condition got sent
}

//I2C Receive
void Receive(int numBytestoRead) {
	PRxData = (unsigned char *) RxBuffer;    // Start of RX buffer
	RXByteCtr = numBytestoRead;              // Load RX byte counter
	while (UCB0CTL1 & UCTXSTP)
		;             // Ensure stop condition got sent
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}

 void I2CWriteBlock(unsigned char tgtAddress, unsigned char *dataToSend, int numBytes) {
	 num_bytes_tx = numBytes;
	 Setup_TX(tgtAddress);
	 Transmit(dataToSend, numBytes);
 }

 void I2CReadBlock(unsigned char tgtAddress, unsigned char *dataRead, int numBytestoRead) {
		Setup_RX(tgtAddress);
		Receive(numBytestoRead);
		while (UCB0CTL1 & UCTXSTP);  // Ensure stop condition got sent
		memcpy(dataRead, RxBuffer, numBytestoRead);
 }
 /**
  * This will read with a repeated start. NumBytestoRead is needed.
  */
void I2CReadBlock(unsigned char tgtAddress, unsigned char reg, unsigned char *dataRead, int numBytestoRead) {

 	MSData[0] = reg;   //Start address
 	Setup_TX(tgtAddress);
 	num_bytes_tx = 2;
 	RPT_Flag = 0;
 	Transmit(MSData, 2);
 	while (UCB0CTL1 & UCTXSTP)
 		;             // Ensure stop condition got sent

 	//Receive
 	Setup_RX(tgtAddress);
 	Receive(numBytestoRead);
	while (UCB0CTL1 & UCTXSTP)
 		;             // Ensure stop condition got sent
 	memcpy(dataRead, RxBuffer, numBytestoRead);
 }


 void initTMP100() {
	 unsigned char data[20];

	 data[0] = 1;
	 I2CWriteBlock(TMP100TGT, data, 1);
	 //12 bit
	 data[0] = 0x60;
	 I2CWriteBlock(TMP100TGT, data, 1);
 }


 void readTMP100() {
	 unsigned char dataRead[10];
	 MSData[0]=0;

	 I2CWriteBlock(TMP100TGT, MSData, 1);

	 I2CReadBlock(TMP100TGT, dataRead, 2);
	 __no_operation();
 }


//Initialize Serial Port
 void initUART() {

    P1SEL |=  BIT1 | BIT2;                     // P1.1 = RXD, P1.2=TXD
 	P1SEL2 |= BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
 	UCA0BR0 = 104;                            // 1MHz 9600
 	UCA0BR0 = 26;							// 1MHz 38400
 	UCA0BR1 = 0;                              // 1MHz 9600 and 38400
 	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
 	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
 	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
 }

