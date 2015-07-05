/* | PUNK PRISM POWER SERVER SCRIPTS <br />
---|---------------------------------*/<br />

repo for the hardware chat for the demo prototype Punk Prism Power. Connects 2 BLE beans using seperate terminal window processes to a chat client
to talk to Unity. 

/* | SETUP (Only for the chat app) <br />
---|---------------------------------*/<br />

Follow the instructions on how to get the LightBlue Bean up and running: https://punchthrough.com/bean/getting-started-osx/

Download and install node: https://nodejs.org/download/<br />
Download and install noble: https://www.npmjs.com/package/noble<br />
Download and install ble-bean: https://www.npmjs.com/package/ble-bean<br />


/* | RUNNING<br />
---|---------------------------------*/<br />

Navigate to the folder where the scripts are kept...<br />

1) Run: node tcpserver.js  (this will start the chat server) in one terminal window <br />
2) Launch two new terminal windows and run: findPlayerOne.js in one and findPlayerTwo.js in the other <br />
   (node findPlayerOne.js / node findPlayer2.js) <br />
   --> You shoudl now have 3 terminal windows running. 1 server and two clients <br />
   --> To test server, you can also telnet into your host and port <br />
3) On the unity side: Make sure you have the socketScript.cs and TCPConnection.cs running <br />
4) Run unity and click connect to server. <br />
	--> You should see a broadcast message when each client connects.<br />
<br />
Start Chattin' !


/* | REFERENCES <br />
---|---------------------------------*/<br />

https://github.com/jacobrosenthal/ble-bean<br />
http://www.hacksparrow.com/tcp-socket-programming-in-node-js.html<br />
https://gist.github.com/creationix/707146<br />


/* | CURRENT ISSUES<br />
---|---------------------------------*/<br />

Its messy because you have to run two processes. But hey, its a chat client. So for now, it will work. <br />