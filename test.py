import paho.mqtt.client as mqtt
import discoveryclient.MQTTDiscoveryClient as DiscoveryClient
import discoveryserver.MQTTDiscoveryServer as DiscoveryServer
import time
import unittest

class MyTest(unittest.TestCase):
  def basic_test(self):
    with DiscoveryServer("discovery.config", "discovery.cache", '192.168.2.5') as server:
      print("Server Connected")
      with DiscoveryClient("Lights", "Smartbulb", '192.168.2.5') as client:
        print("Client Connected")
        client.disovery_query()
        time.sleep(1000)
        self.assertEqual(1, server.pending.length)
        server.add_client(client.name, "Lights", "DiscoLamp")
        time.sleep(1000)
        self.assertTrue(client.name.startswith("DiscoLamp"))
