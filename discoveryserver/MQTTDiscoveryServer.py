import paho.mqtt.client as mqtt
import json
import pickle

class MQTTDiscoveryServer:
  """Implements the MQTTDiscovery Protocol"""
  def __init__(self, config_file, cache_file, mqtt_ip, mqtt_port = 1883):
    self.mqtt = mqtt.Client()
    self.mqtt.on_connect = self.on_connect
    self.mqtt.on_message = self.on_message
    self.mqtt_ip = mqtt_ip
    self.mqtt_port = mqtt_port
    self.cache_file = cache_file
    self.config_file = config_file
    self.pending = []
    self.clients = {}
    self.serial_index = 1
    self.default_name = "MQTT_Client"
    self.read_config()
    self.read_clients()
    self.connected = False

  def __enter__(self):
    self.connect()
    return self
  
  def __exit__(self, exc_type, exc_value, traceback):
    self.mqtt.loop_stop(True)
    self.mqtt.disconnect()

  def read_config(self):
    with open(self.config_file, 'r') as f:
      s = f.read()
      self.serial_index = json.loads(s)['serial_index']

  def write_config(self):
    with open(self.config_file, 'w') as f:
      f.write(son.dumps({'serial_index': self.serial_index}, sort_keys=True,indent=2))

  def read_clients(self):
    with open(self.cache_file, 'r') as f:
      self.categories = pickle.load(f)

  def write_clients(self):
    with open(self.cache_file, 'w') as f:
      pickle.dump(self.categories, f, pickle.HIGHEST_PROTOCOL)

  def add_client(self, discovery_name, category, client_name):
    self.pending.remove(discovery_name)
    new_client ={'name': client_name, 'serial': self.serial_index}
    self.serial_index += 1
    self.write_config()
    # Get the Enumerator by finding the lowest free enum with client name
    enums = []
    for client in self.clients['category']:
      enums.append(client['enum'])
    new_enum = 1
    enums = sorted(enums)
    for enum in enums:
      if enum == new_enum:
        new_enum += 1
      else:
        break
    new_client['enum'] = new_enum
    self.clients['category'].append(new_client)
    self.write_clients()
    self.mqtt.publish("/discover",
      "{\"client\":\"{0}\", \"serial\":{1}, \"name\": \"{2}\", \"enum\":{3}, \"request\": \"initialization\"}".format(
        discovery_name,
        new_client['serial'],
        new_client['name'],
        new_client['enum'])
      ,2)

  def connect(self):
    self.mqtt.connect(self.mqtt_ip, self.mqtt_port, 60)

  def on_connect(self):
    print("Connect: "+str(rc))
    self.mqtt.subscribe("/discover")
    self.mqtt.loop_start()

  def on_message(client, userdata, msg):
    data = json.loads(str(msg.payload))
    if 'discovery' in data:
      self.pending.append(data['discovery'])
