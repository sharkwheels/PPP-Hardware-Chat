/* | CLIENT PROCESS 2 FOR PLAYER 2
---|---------------------------------*/

"use strict";

/* | DEPENDENCIES
---|---------------------------------*/

var Sceptre = require('ble-bean');

/* | GLOBALS
---|---------------------------------*/

var intervalId;
var connectedBean;
var uuidK = '950ff3b2687d4f4ab534ef8550062597'; // knuckles Bean

/* | CONNCTION VARIABLES
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
    
  var beanName = 'playerTwo';

  console.log('connected to: ' + bean.uuid);
  console.log('bean Name: ' + beanName);

  // Read the serial data, compare what's there and write the corresponding thing to the server
  bean.on("serial", function(data, valid){
      console.log("received serial:\t", data.toString('hex'), data.toString('utf8'));
      var beanCo = data.toString('utf8');
      client.write('p2slash'+'\n');
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

/* | EXIT HANDLER
---|---------------------------------*/

// THIS IS MESSY

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
