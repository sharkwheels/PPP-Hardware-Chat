/* | CLIENT PROCESS 1 FOR PLAYER 1
---|---------------------------------*/

"use strict";

/* | Dependencies
---|---------------------------------*/

var Sceptre = require('ble-bean');

///// note, ble-bean is a library that runs on top of noble. you will need to also install noble. 


/* | GLOBALS
---|---------------------------------*/

var intervalId;
var connectedBean;
var uuidK = '921e9cc7a6f14af8be0a13fe79ac916c'; // Sceptre Bean


/* | CONNECTION VARIABLES
---|---------------------------------*/

var net = require('net');
var HOST = '127.0.0.1';
var PORT =  '3000';
var client = new net.Socket();
var totallyConnected = false;

/* | DISCOVER BEAN AND WRITE TO SOCKET
---|---------------------------------*/

Sceptre.discoverByUuid(uuidK,function(bean){
  connectedBean = bean;
  process.on('SIGINT', exitHandler.bind(this));
    
  var beanName = 'playerOne';

  console.log('connected to: ' + bean.uuid);
  console.log('bean Name: ' + beanName);

  // Read the serial data, compare what's there and write the corresponding thing to the server
  bean.on("serial", function(data, valid){
      console.log("received serial:\t", data.toString('hex'), data.toString('utf8'));
      client.write('p1slash'+'\n');
  });

  bean.on("disconnect", function(){
    process.exit();
  });

  bean.connectAndSetup(function(){
    // read a notify here.
    // use scratch data if you want
  });
});

/* | SERVER CONNECTION
---|---------------------------------*/

client.connect(PORT, HOST, function(){
  console.log('Connected to: ' + HOST + ': ' + PORT);
  totallyConnected = true;
});

client.on('data', function(data) {
      console.log('DATA: '+data);
});

client.on('error', function(err){
      if (err) throw err;
});

client.on('close', function() {
      console.log('Connection closed');
      triedToExit = true;
});


/* | KEEP THE PROGRAM LOOPING
---|---------------------------------*/

process.stdin.resume();//so the program will not close instantly
var triedToExit = false;

/* | EXIT HANDLERS 
---|---------------------------------*/

// this is messy. 

var exitHandler = function exitHandler() {

  var self = this;

  if (connectedBean && !triedToExit) {
    triedToExit = true;
    console.log('Turning off led...');
    clearInterval(intervalId);
    connectedBean.setColor(new Buffer([0,0,0]), function(){});
    //no way to know if succesful but often behind other commands going out, so just wait 3 seconds
    console.log('Disconnecting from Device...');
    setTimeout(connectedBean.disconnect.bind(connectedBean, function(){}), 3000);
  } else {
    process.exit();
  }
};
