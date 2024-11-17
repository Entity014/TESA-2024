// > node client2.js
const WebSocket = require('ws');
const mqtt = require('mqtt');

// Connect to WebSocket server
const ws = new WebSocket('ws://localhost:8081');

// Connect to MQTT Broker
const mqttClient = mqtt.connect('mqtt://password:username@localhost:1883/');

// Handle WebSocket messages from the server
ws.on('message', (message) => {
  console.log('Received from WebSocket:', message);

  // Check if the received message is the expected one
  if (message === 'Trigger to send {"hardware": "sent"} to MQTT') {
    // Publish a specific message to MQTT topic "hardware/sent"
    const mqttMessage = { "hardware": "sent" };

    mqttClient.publish('hardware/sent', JSON.stringify(mqttMessage), () => {
      console.log('Message sent to MQTT: ', mqttMessage);
    });
  }
  
  // Check if the received message is the expected one
  if (message === 'Trigger to send {"hardware": "sent1"} to MQTT') {
    // Publish a specific message to MQTT topic "hardware/sent"
    const mqttMessage = { "hardware": "sent" };
    
    mqttClient.publish('hardware/sent', JSON.stringify(mqttMessage), () => {
      console.log('Message sent to MQTT: ', mqttMessage);
    });
  }
  
  if (message === 'Trigger to send {"hardware": "sent2"} to MQTT') {
    // Publish a specific message to MQTT topic "hardware/sent"
    const mqttMessage = { "hardware": "sent2" };
    
    mqttClient.publish('hardware/sent', JSON.stringify(mqttMessage), () => {
      console.log('Message sent to MQTT: ', mqttMessage);
    });
  }

  if (message === 'Trigger to send {"hardware": "true"} to MQTT') {
    // Publish a specific message to MQTT topic "hardware/sent"
    const mqttMessage = { "hardware": "true" };
    
    mqttClient.publish('hardware/sent', JSON.stringify(mqttMessage), () => {
      console.log('Message sent to MQTT: ', mqttMessage);
    });
  }

  if (message === 'Trigger to send {"hardware": "false"} to MQTT') {
    // Publish a specific message to MQTT topic "hardware/sent"
    const mqttMessage = { "hardware": "false" };
    
    mqttClient.publish('hardware/sent', JSON.stringify(mqttMessage), () => {
      console.log('Message sent to MQTT: ', mqttMessage);
    });
  }
});

// Connect to MQTT Broker
mqttClient.on('connect', () => {
  console.log('Connected to MQTT broker');
});
