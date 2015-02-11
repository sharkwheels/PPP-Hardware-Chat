
"use strict";

var Sceptre = require('ble-bean');
var intervalId;
var connectedBean;
var uuidK = '950ff3b2687d4f4ab534ef8550062597'; // knuckles Bean

var net = require('net');
var HOST = '127.0.0.1';
var PORT =  '3000';

//client to Server Connection vars
var client = new net.Socket();
var totallyConnected = false;

// Discover and read from the virtual serial port

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


// server host connection

client.connect(PORT, HOST, function(){
  console.log('Connected to: ' + HOST + ': ' + PORT);
  totallyConnected = true;
});

client.on('data', function(data) {
      console.log('DATA: '+data);
});

client.on('error', function(err){
      // do nothing yo.
      // eventually log out to file
});

client.on('close', function() {
      console.log('Connection closed');
      triedToExit = true;
});


//// Looping 

process.stdin.resume();//so the program will not close instantly
var triedToExit = false;

//// Exit 
//turns off led before disconnecting

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
