import paho.mqtt.client as mqtt
import random
import json

class MQTTDiscoveryClient:
  """Implements the MQTTDiscovery Protocol"""
  def __init__(self, category, name, mqtt_ip, mqtt_port = 1883):
    self.mqtt_ip = mqtt_ip
    self.mqtt_port = mqtt_port
    self.mqtt = mqtt.Client()
    self.mqtt.on_connect = self.on_connect
    self.mqtt.on_message = self.on_message
    self.discovered = False
    self.serial = 0
    self.on_discovered = None
    self.on_named = None
    # Build the client
    random.seed()
    self.name = category + "-" + name + "-" + str(random.randint(1,4294967295))

  def __enter__(self):
    print
    self.connect()
    return self
  
  def __exit__(self, exc_type, exc_value, traceback):
    self.mqtt.loop_stop(True)
    self.mqtt.disconnect()

  def connect(self):
    self.mqtt.connect(self.mqtt_ip, self.mqtt_port, 60)

  def on_connect(self):
    print("Connect: "+str(rc))
    self.mqtt.subscribe("/discover")
    self.mqtt.loop_start()

  def discover_query(self):
    self.mqtt.publish("/discover",  "{\"discovery\": \"" + name + "\",\"request\":\"register\"}", 2)

  def name_query(self):
    self.mqtt.publish("/discover", "{\"serial\":" + str(serial) + ", \"request\":\"name\"}", 2)

  def on_message(self, client, userdata, msg):
    data = json.loads(str(msg.payload))
    if 'client' in data and 'serial' in data and 'name' in data and 'enum' in data:
      if data['client'] == self.name:
        self.serial = serial
        self.name = data['name'] + '-' + data['enum']
        if self.on_discovered:
          self.on_discovered(self.serial, self.name)
        if self.on_named:
          self.on_named(self.name)
    else:
      if 'serial' in data and data['serial'] == self.serial and 'name' in data and 'enum' in data:
        self.name = data['name'] + '-' + data['enum']
        if self.on_named:
          self.on_named(self.name)
