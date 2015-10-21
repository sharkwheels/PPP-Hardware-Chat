<h2>PUNK PRISM POWER SERVER SCRIPTS</h2>


repo for the hardware chat for the demo prototype Punk Prism Power. Connects 2 BLE beans using seperate terminal window processes to a chat client
to talk to Unity. 

<h2> Setup (Only for the chat app) </h2>

Follow the instructions on how to get the LightBlue Bean up and running: https://punchthrough.com/bean/getting-started-osx/ </br />

Download and install node: https://nodejs.org/download/<br />
Download and install noble: https://www.npmjs.com/package/noble<br />
Download and install ble-bean: https://www.npmjs.com/package/ble-bean<br />


<h2>Running It</h2>

Navigate to the folder where the scripts are kept...<br />

1) Run: node tcpserver.js  (this will start the chat server) in one terminal window <br />
2) Launch a new terminal tab (apple + t) and run: findPlayers2.js (node findPlayers.js) <br />
   --> You should now have 2 terminal windows running. 1 server and one client process <br />
   --> To test server, you can also telnet into your host and port <br />
3) On the unity side: Make sure you have the socketScript.cs and TCPConnection.cs running <br />
4) Run unity and click connect to server. <br />
	--> You should see a broadcast message when each client connects.<br />
<br />
Start Chattin' !


<h2>References</h2>

http://www.hacksparrow.com/tcp-socket-programming-in-node-js.html<br />
https://gist.github.com/creationix/707146<br />
http://bildr.org/2011/04/sensing-orientation-with-the-adxl335-arduino/</br />


<h2>Current Issues</h2>

There is some extra TCP Garble coming through that I can't currently figure out. You'll have to write a filter on the other side. <br />