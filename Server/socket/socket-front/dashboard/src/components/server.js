// > node server.js
const WebSocket = require('ws');
const express = require('express');
const app = express();
const http = require('http').Server(app);
const wss = new WebSocket.Server({ server: http });

// When a WebSocket client (App.js) connects
wss.on('connection', (ws) => {
  console.log('Client connected to WebSocket server');

  // Listen for messages from App.js
  ws.on('message', (message) => {
    console.log('Received message from App.js:', message);


    // Forward the message to Client 2 (if Client 2 is connected)
    wss.clients.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message);  // Send message to Client 2
      }
    });
  });
});

// Start the HTTP and WebSocket server
http.listen(8081, () => {  // Change port to 8081
  console.log('WebSocket server is running on ws://localhost:8081');
});
