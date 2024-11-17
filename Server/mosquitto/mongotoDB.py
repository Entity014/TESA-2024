import paho.mqtt.client as mqtt
import pymongo
from pymongo import MongoClient
import json

# MongoDB setup
client = MongoClient("mongodb://password:username@localhost:27017/")
db = client["mydb"]
collection = db["mqttlog"]

# MQTT settings
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "hardware/data"

# Callback function when a message is received
def on_message(client, userdata, message):
    message_payload = message.payload.decode('utf-8')
    print(f"Received message: {message_payload}")
    
    try:
        # Convert payload string to a JSON object
        data = json.loads(message_payload)
        
        # Assuming the payload has the format you mentioned
        # the_data = {
        #     "id": data.get("id"),
        #     "fullname": data.get("fullname"),
        #     "major": data.get("major"),
        #     "gpa": data.get("gpa")
        # }

        # Insert data into MongoDB
        collection.insert_one(data)
        print("Data inserted into MongoDB.")
    
    except json.JSONDecodeError:
        print("Error decoding the message payload.")

# Set up MQTT client
client = mqtt.Client()
client.on_message = on_message

# Connect to MQTT broker
client.username_pw_set("password", "username")
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Subscribe to the topic
client.subscribe(MQTT_TOPIC)

# Start the loop to process messages
client.loop_forever()
