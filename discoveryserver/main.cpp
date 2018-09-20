#include "MQTTDiscoveryServer.h"

int main(int argc, void **argv) {
  bool running = true;
  if (argc < 2) {
    printf("usage %s mqtt_host <mqtt_port>", (char*)argv[0]);
  }
  int port = 1883;
  if (argc == 3) {
    port = atoi((char*)argv[2]);
  }
  int res = 0;
  MQTTDiscoveryServer mqtt("discovery.config", "discovery.cache", (char*)argv[1], port);
  while(running) {
    res = mqtt->loop();
    if (res)
      mqtt->reconnect();
    usleep(1e5);
  }
}