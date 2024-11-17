// client1.js (MQTT Client)
const mqtt = require('mqtt');
const WebSocket = require('ws');

// Connect to MQTT Broker
const mqttClient = mqtt.connect('mqtt://password:username@localhost:1883/');

// Connect to WebSocket server (same server that will handle App.js)
const ws = new WebSocket('ws://localhost:8081');

// MQTT Topic to Subscribe to
mqttClient.on('connect', () => {
  console.log('Connected to MQTT broker');
  mqttClient.subscribe('hardware/log', (err) => {
    if (!err) {
      console.log('Subscribed to hardware/log');
    }
  });
});

// When MQTT receives a message, send it to the WebSocket server
mqttClient.on('message', (topic, message) => {
  console.log(`Received from MQTT topic: ${topic}, message: ${message}`);
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(message.toString());  // Forward the MQTT message to WebSocket server
  }
});

