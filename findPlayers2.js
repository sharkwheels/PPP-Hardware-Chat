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
var allowDuplicates = false; // https://github.com/sandeepmistry/noble/issues/65
//var beanMap = {};

/* | Bean Specific Service UUID
---|---------------------------------*/

var serviceUUID = 'a495ff10c5b14b44b5121370f02d74de';  // primary service UUID, holds
var beanTransportChar = 'A495FF11C5B14B44B5121370F02D74DE'

// var sceptreUUID = a78be07269c24e26b6242ea9a3e97535;
// var chainsawUUID = c111d0df236440b0aff54d5b4ab7bfde;
//var specificBeans = ["Sceptre", "Chainsaw", "Xmas", "Bean"];


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
		noble.startScanning([serviceUUID], allowDuplicates);
		console.log("BLE enabled, scanning started");
	} else {
		noble.stopScanning();
		console.log("BLE is off, scanning stopped");
	}
});




/* | DISCOVER BEAN AND CONNECT 
---|---------------------------------*/

// got rid of the bind
// got rid of beanmap...still works.
// moved serviceUUID filter for beans to scan instead of here


noble.on('discover', function(q) {
	console.log("found bean:" + q.advertisement.localName + " - UUID: " + q.uuid);
	
	var bn = q.advertisement.localName;
	var beanName = bn.toLowerCase();
	var beanUUID = q.uuid;

	if (_.contains(['chainsaw','sceptre'], beanName)) {
    	console.log("!contains",beanName);

    	q.connect(function(err) {
    		if (err) throw err;
			console.log("!connect", beanName);
			setupServices(beanName, q);

			// https://github.com/sandeepmistry/noble/issues/77  <-- trying once instead of on.

			q.once('disconnect', function(){
				console.log(q.advertisement.localName + " disconnected, trying to find it...");
				noble.startScanning([serviceUUID],allowDuplicates);
			});
    	});

	}else{
		console.log("not the bean you want");
	}

});



/* | SETUP SERIAL SERVICE
---|---------------------------------*/

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
                    //beanMap[beanName].characteristics = characteristics;
				});
			}

		});
	});

};


/* | READ FROM BEAN
---|---------------------------------*/

var readFromBean = function(beanName, serialTransport) {
	console.log("!readFromBean", beanName);

	serialTransport.on('read', function(data, isNotification) {
			
			var burp = data; // get that data and shuffle it over 8 bits. [1]<<8 || (data[0])
			var value = burp.toString('utf8'); // make it a string ut8f
			var trimmed = value.trim();

			// send it along to the server
			//console.log(trimmed + "\n");
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


/* | RESUME PROCESS
---|---------------------------------*/

process.stdin.resume(); //so the program will not close instantly


/* | EXIT HANDLER
---|---------------------------------*/

var exitHandler = function exitHandler(options, err) {

	if (options.cleanup) console.log('Closing now!');
    if (err) console.log(err.stack);
    if (options.exit) process.exit();
};

//do something when app is closing
process.on('exit', exitHandler.bind(null,{cleanup:true}));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null, {exit:true}));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null, {exit:true}));

