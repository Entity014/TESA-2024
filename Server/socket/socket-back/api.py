"""
from flask import Flask , request , jsonify , make_response

app = Flask(__name__)

data_store = {}

@app.route("/")
def index():
    return "<p>Hello, World!</p>"

@app.route("/data",methods=["GET"])
def get_data():
    data={"key1":"value1" , "key2":"value2", "key3":"value3"}
    return jsonify(data),200


@app.route("/data", methods=["POST"])
def create_data():
    new_data = request.get_json()  # Expecting JSON data in the request
    data_id = new_data.get("id")
    data_store[data_id] = new_data
    return jsonify({"message": "Data added", "data": new_data}), 201

# PUT: Update data (or create if not exist)
@app.route("/data/<data_id>", methods=["PUT"])
def update_data(data_id):
    updated_data = request.get_json()
    data_store[data_id] = updated_data
    return jsonify({"message": "Data updated", "data": updated_data}), 200

# PATCH: Partially update data
@app.route("/data/<data_id>", methods=["PATCH"])
def patch_data(data_id):
    data = data_store.get(data_id)
    if not data:
        return jsonify({"message": "Data not found"}), 404
    
    # Partially update only the fields provided in the request
    updates = request.get_json()
    data.update(updates)
    return jsonify({"message": "Data partially updated", "data": data}), 200

# DELETE: Remove data
@app.route("/data/<data_id>", methods=["DELETE"])
def delete_data(data_id):
    if data_id in data_store:
        del data_store[data_id]
        return jsonify({"message": "Data deleted"}), 200
    return jsonify({"message": "Data not found"}), 404

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=5000,debug=True)
"""

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("prediction/loop")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.username_pw_set("password","username")
mqttc.connect("host.docker.internal", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
mqttc.loop_forever()
