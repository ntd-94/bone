#!/usr/bin/env node
'use strict';

const MYIP = '192.168.1.102';

var tgtIpAddress; 
var portNumber = 11000;
const FauxMo = require('fauxmojs');
var Gpio = require('onoff').Gpio,
    led = new Gpio(17, "out"),
    cb;
var process = require('process'); 
var childProcess = require('child_process'); 

/*
process.argv.forEach((val, index) => {
  console.log(`${index}: ${val}`);
});
*/

   if (process.argv[2]) {
   tgtIpAddress = process.argv[2];
   } else {
   tgtIpAddress = MYIP; 
   }


let fauxMo = new FauxMo({
    ipAddress: tgtIpAddress,
    devices: [{
            name: 'office light',
            port: portNumber++,
            handler: (action) => {
                console.log('office light action:', action);
            }
        },
        {
            name: 'pie light',
            port: portNumber++,
            handler: (action) => write(action)
        },
        {
            name: 'Andy light',
            port: portNumber++,
            handler: (action) => write(action)
        },
        {
            name: 'Socket One',
            port: portNumber++,
            handler: (action) => writeSocket('1', action)
        },
        {
            name: 'Socket Two',
            port: portNumber++,
            handler: (action) => writeSocket('2', action)
        },
        {
            name: 'Socket Three',
            port: portNumber++,
            handler: (action) => writeSocket('3', action)
        },
        {
            name: 'Socket Four',
            port: portNumber++,
            handler: (action) => writeSocket('4', action)
        },
    ]
});

console.log('Started on IP %s. Waiting for command.', tgtIpAddress);


// functions

function message(action) {
    console.log("Office log", action);
}


function write(action) {
    if (action == 'off') {
        led.writeSync(0);
    } else {
        led.writeSync(1);
    }
    console.log('State written to pin: %s', action);
}


function writeSocket(deviceNumber, action) {
    childProcess.exec(`/home/andy/bone/433Utils/RPi_utils/pwr.sh  ${deviceNumber} ${action} `, (error, stdout, stderr) => {
  if (error) {
    console.error(`exec error: ${error}`);   
    return;
  }
  console.log(`stdout: ${stdout}`); 
  console.log(`stderr: ${stderr}`); 
  }); 
}

