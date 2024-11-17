import time
import paho.mqtt.client as mqtt
import json

def on_publish(client, userdata, mid, reason_code, properties):
    # Handle acknowledgment for published messages
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
mqttc.username_pw_set("password", "username")  # Replace with actual credentials
mqttc.connect("host.docker.internal", 1883, 60)

# Loop to send a structured JSON message every 3 seconds
i = 0
while True:
    message = json.dumps({
        "_id": i,
        "fullname": f"Mos{i}",  # e.g., "Mos1", "Mos2", etc.
        "major": f"REAI{i}",    # e.g., "REAI1", "REAI2", etc.
        "gpa": 4
    })
    
    mqttc.publish("prediction/loop", message, qos=0)
    print("Published message:", message)  # Optional: log the message
    time.sleep(3)
    i += 1
