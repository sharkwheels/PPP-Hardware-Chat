"use strict";

/* | Dependencies
---|---------------------------------*/

var noble = require('noble');  // drives the BLE process
var _     = require('lodash'); // ??
var util  = require('util'); // needed for socker support
var net = require('net');

/* | Global Variables
---|---------------------------------*/

var exitHandlerBound = false;
var beanMap = {};

/* | Bean Specific Service UUID
---|---------------------------------*/

var serviceUUID = 'a495ff10c5b14b44b5121370f02d74de';  // primary service UUID, holds
var beanTransportChar = 'A495FF11C5B14B44B5121370F02D74DE'

// Sceptre UUID: a78be07269c24e26b6242ea9a3e97535
// Chainsaw UUID: c111d0df236440b0aff54d5b4ab7bfde


/* | CHAT SERVER CONNECTION VARIABLES
---|------------------------------------*/

var HOST = '127.0.0.1';
var PORT =  '3000';
var client = new net.Socket();


/************ | FUNCTIONS
-----------------------------|--------------------------------*/

/* | BLE State Change Detection
---|---------------------------------*/

noble.on('stateChange', function(state){
	if (state === 'poweredOn'){
		noble.startScanning();
		console.log("BLE enabled, scanning started");
	} else {
		noble.stopScanning();
		console.log("BLE is off, scanning stopped");
	}
});


/* | DISCOVER BEANS
---|---------------------------------*/

var discoverBeans = function(q) {

	// discover all the beans

	if (_.contains(q.advertisement.serviceUuids, serviceUUID)) {		
		console.log("found bean:" + q.advertisement.localName + " - UUID: " + q.uuid);
		// this makes an object w/ the advertisment / services of the bean
    	beanMap[q.advertisement.localName] = q.advertisement;
    	//console.log("!discover", beanMap);  	
    	q.connect(connect.bind({q:q})); 
	}else {
		console.log("not a bean");
	}
	
};


/* | CONNECT TO BEANS AND SETUP SERIAL SERVICE
---|---------------------------------------------*/

var connect = function(err) {
	if (err) throw err;
	console.log("Connection to " + this.q.uuid);

	var thisBean = this.q;
	var beanName = thisBean.advertisement.localName;

	console.log("!connect", beanName);

	thisBean.on('disconnect', function(){
		console.log(thisBean.advertisement.localName + " disconnected, trying to find it...");
		noble.startScanning();
	});
	setupServices(beanName, thisBean);

};

var setupServices = function(beanName, thisBean) {
	console.log("!setupServices:", beanName);
	thisBean.discoverServices(null, function(err, services) {
		services.forEach(function(service){
			// find the UUID for the serial service. Bean Transport: a495ff11c5b14b44b5121370f02d74de  A495FF11C5B14B44B5121370F02D74DE
			// set thtat up
			if (service.uuid === serviceUUID) {
				console.log("found bean services");
				var characteristicUUIDs =[beanTransportChar];
				service.discoverCharacteristics(characteristicUUIDs, function(err, characteristics) { 
					if (err) throw err;
					console.log("!service.discovered ", beanName + " serial services");
					// send that to read and write
					readFromBean(beanName, characteristics[0]); // name, serial transport
                    beanMap[beanName].characteristics = characteristics;
				});
			}

		});
	});

};


/* | READ FROM BEAN
---|---------------------------------*/

var readFromBean = function(beanName, serialTransport) {
	console.log("!readFromBean", beanName, serialTransport);

	serialTransport.on('read', function(data, isNotification) {
			
			var burp = data; // get that data and shuffle it over 8 bits. [1]<<8 || (data[0])
			var value = burp.toString('utf8'); // make it a string ut8f
			var trimmed = value.trim();

			// send it along to the server
			client.write(trimmed + "\n");
			
		});

		serialTransport.notify(true, function(err) {
			if (err) throw err;
		});
};

/* | SERVER CONNECTION
---|----------------------------------------------------*/


client.connect(PORT, HOST, function(){
	console.log('Connected to: ' + HOST + ': ' + PORT);
});

client.on('data', function(data) {
	console.log('DATA: '+ data);
});

client.on('error', function(err){
	if (err) {
		console.log(err);
	}
});

client.on('close', function() {
	console.log('Connection closed');
});	




/* | NOBLE PROCESS
---|---------------------------------*/

// only find beans with a particular service UUID (scratch)
noble.startScanning([serviceUUID], false);
noble.on('discover', discoverBeans);
process.stdin.resume(); //so the program will not close instantly


/* | EXIT HANDLER
---|---------------------------------*/

var exitHandler = function exitHandler(options, err) {

	if (options.cleanup) console.log(' am closing now! ');
    if (err) console.log(err.stack);
    if (options.exit) process.exit();
};

//do something when app is closing
process.on('exit', exitHandler.bind(null,{cleanup:true}));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, {exit:true}));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, {exit:true}));

