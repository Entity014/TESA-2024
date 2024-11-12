import time
import paho.mqtt.client as mqtt

def on_publish(client, userdata, mid, reason_code, properties):
    # reason_code and properties will only be present in MQTTv5. It's always unset in MQTTv3
    try:
        userdata.remove(mid)
    except KeyError:
        print("on_publish() is called with a mid not present in unacked_publish")
        print("This is due to an unavoidable race-condition:")
        print("* publish() return the mid of the message sent.")
        print("* mid from publish() is added to unacked_publish by the main thread")
        print("* on_publish() is called by the loop_start thread")
        print("While unlikely (because on_publish() will be called after a network round-trip),")
        print(" this is a race-condition that COULD happen")
        print("")
        print("The best solution to avoid race-condition is using the msg_info from publish()")
        print("We could also try using a list of acknowledged mid rather than removing from pending list,")
        print("but remember that mid could be re-used !")

unacked_publish = set()
mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_publish = on_publish

mqttc.user_data_set(unacked_publish)
mqttc.username_pw_set("password","username")
mqttc.connect("host.docker.internal", 1883, 60)

#Our application produce some messages
msg_info = mqttc.publish("prediction/device1", "my message", qos=0)

#Wait for all message to be published
i = 0
while True:
    mqttc.publish("prediction/loop", i , qos=0)
    time.sleep(3)
    i = i + 1 
    
# import time
# import paho.mqtt.client as mqtt
# import json

# def on_publish(client, userdata, mid, reason_code=None, properties=None):
#     # Handle acknowledgment for published messages
#     try:
#         userdata.remove(mid)
#     except KeyError:
#         print("on_publish() is called with a mid not present in unacked_publish")
#         print("This is due to an unavoidable race-condition:")

# unacked_publish = set()
# mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
# mqttc.on_publish = on_publish

# mqttc.user_data_set(unacked_publish)
# mqttc.username_pw_set("password", "usernames")  # Replace with actual credentials
# mqttc.connect("host.docker.internal", 1883, 60)

# # Our application produces messages
# msg_info = mqttc.publish("prediction/device1", "my message", qos=0)

# # Loop to send {"gpa": i} every 3 seconds
# i = 0
# while True:
#     message = json.dumps({"gpa": i})  # Convert dictionary to JSON string
#     mqttc.publish("prediction/loop", message, qos=0)
#     time.sleep(3)
#     i += 1
