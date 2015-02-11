
var net = require('net');
var HOST = '127.0.0.1';
var PORT =  '3000';
var clients = [];

// Start a TCP Server
net.createServer(function (socket) {

  // Identify this client
  socket.name = socket.remoteAddress + ":" + socket.remotePort 

  // Put this new client in the list
  clients.push(socket);
  //console.log(clients);

  // Send a nice welcome message and announce
  socket.write('connected: ' + socket.name + '\n');
  broadcast(socket.name + 'joined the chat\n', socket);


  // Handle incoming messages from clients.
  socket.on('data', function (data) {
    //broadcast(socket.name +': '+ data + '\n');
    broadcast(data);
    //socket.pipe(data);
    //console.log(clients);

  });

  // Remove the client from the list when it leaves
  socket.on('end', function () {
    clients.splice(clients.indexOf(socket), 1);
    broadcast(socket.name + ' disconnected.\n');
  });
  
  // Send a message to all clients except the sender
  function broadcast(message, sender) {
    clients.forEach(function (client) {
      // Don't want to send it to sender
      if (client === sender) return;
      client.write(message);
    });
    // Log it to the server output too
    process.stdout.write(message)
  }

}).listen(PORT);

// Put a friendly message on the terminal of the server.
console.log('Hardware Chat Client\n');
