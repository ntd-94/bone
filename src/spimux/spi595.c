/** SPI C Transfer Example, Written by Derek Molloy (www.derekmolloy.ie)
*    for the book Exploring BeagleBone. Based on the spidev_test.c code
*    example at www.kernel.org
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<linux/spi/spidev.h>

#define SPI_PATH "/dev/spidev1.0"
//#define SPI_PATH "/dev/spidev2.0"

int
transfer (int fd, unsigned char send[], unsigned char receive[], int length)
{
  struct spi_ioc_transfer transfer;	//the transfer structure
  transfer.tx_buf = (unsigned long) send;	//the buffer for sending data
  transfer.rx_buf = (unsigned long) receive;	//the buffer for receiving data
  transfer.len = length;	//the length of buffer
  transfer.speed_hz = 1000000;	//the speed in Hz
  transfer.bits_per_word = 8;	//bits per word
  transfer.delay_usecs = 0;	//delay in us

  // send the SPI message (all of the above fields, inc. buffers)
  int status = ioctl (fd, SPI_IOC_MESSAGE (1), &transfer);
  if (status < 0)
    {
      perror ("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
    }
  return status;
}

int
main ()
{
  unsigned int fd, i = 0;	//file handle and loop counter
  unsigned char value, null = 0x00;	//sending only a single char
  uint8_t bits = 8, mode = 3;	//8-bits per word, SPI mode 3
  uint32_t speed = 1000000;	//Speed is 1 MHz
  int j;
  unsigned char cSendData[50];
  unsigned char cReceiveData[50];

  // The following calls set up the SPI bus properties
  if ((fd = open (SPI_PATH, O_RDWR)) < 0)
    {
      perror ("SPI Error: Can't open device.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_MODE, &mode) == -1)
    {
      perror ("SPI: Can't set SPI mode.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_MODE, &mode) == -1)
    {
      perror ("SPI: Can't get SPI mode.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
    {
      perror ("SPI: Can't set bits per word.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1)
    {
      perror ("SPI: Can't get bits per word.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
      perror ("SPI: Can't set max speed HZ");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
    {
      perror ("SPI: Can't get max speed HZ.");
      return -1;
    }

  // Check that the properties have been set
  printf ("SPI Mode is: %d\n", mode);
  printf ("SPI Bits is: %d\n", bits);
  printf ("SPI Speed is: %d\n", speed);
  printf ("Counting through all of the LEDs:\n");

  memset (cReceiveData, 0, 50);
  for (j = 0; j < 1000; j++)
    {
      for (i = 0; i < 6100; i++)
	{
	  cSendData[0] = (i & 0xFF);
	  cSendData[1] = (char) ((i >> 8) & 0xFF);
	  cSendData[2] = 5;
	  cSendData[3] = 8;
	  cSendData[4] = 10 & 0xff;
	  cSendData[5] = 12;

	  // This function can send and receive data, just sending here
	  if (transfer
	      (fd, (unsigned char *) &cSendData[0], &cReceiveData[0],
	       5) == -1)
	    {
	      perror ("Failed to update the display");
	      return -1;
	    }
	  fflush (stdout);	//need to flush the output, no \n
	}
    }
  i = 0;
  transfer (fd, (unsigned char *) &i, &null, 1);
  close (fd);			//close the file
  return 0;
}