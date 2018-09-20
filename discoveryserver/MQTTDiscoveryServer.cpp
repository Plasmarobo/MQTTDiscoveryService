#include <mosquittopp.h>
#include <string>
#include <map>
#include <iostream>
#include "MQTTDiscoveryServer.h"

  struct DiscoveryClient {
    std::string category;
    std::string name;
    uint32_t enum;
    uint32_t serial;
  };
MQTTDiscoveryServer::MQTTDiscoveryServer(const char* config_file,
  const char* cache_file,
  const char* host,
  int port=1883) : mosquittopp("DiscoveryService") {
    mosqpp::lib_init();

    connect(host, post, KEEPALIVE_SEC);
    read_config();
    read_clients();
}

MQTTDiscoveryServer::~MQTTDiscoveryServer() {

}

void MQTTDiscoveryServer::on_connect(int rc) {
  printf("Connected: %d\n", rc);
  if (rc == 0) {
    subscribe(NULL, "/discovery");
  }
}

void MQTTDiscoveryServer::on_message(const struct mosquitto_message *message) {

}

void MQTTDiscoveryServer::on_subscribe(int mid, int qos_count, cont int *granted_qos) {

}

void MQTTDiscoveryServer::add_client(std::string discovery_name, std::string category, std::string client_name) {
  DiscoveryClient client;
  for(std::list<std::string>::iterator i = _pending.begin();
      i != _pending.end();
      ++i) {
    if (*i == discovery_name) {
      _pending.remove(c);
      break;
    }
  }
  uint32_t enumerator = 1;
  for(auto c : _clients.values()) {
    if(c.name == client_name && c.enumerator == enumerator) {
      ++enumerator;
    }
  }
  client.serial = serial_index++;
  client.name = client_name;
  client.enumerator = enumerator;
  client.category = category;
  _clients[client.serial] = client;
}

void MQTTDiscoveryServer::write_config() {
  std::ofstream file(_config_file, std::ofstream::out);
  file << "serial_index=";
  file << serial_index;
  file << std::endl;
  file.close();
}

void MQTTDiscoveryServer::write_cache() {
  std::ofstream file(_cache_file, std::ofstream::out);
  for(auto client : _clients.values()) {
    client.write(file);
  }
  file.close();
}

void MQTTDiscoveryServer::read_config() {
  std::ifstream file(_config_file, std::ifstream::in);
  std::string buffer;
  file >> buffer;
  size_t key_index = buffer.find_first_of('=');
  std::string key = buffer.substr(0, key_index);
  std::string value = buffer.substr(key_index+1);
  if (key == "serial_index") {
    _serial_index = atoi(value.c_str());
  }
}

void MQTTDiscoveryServer::read_cache() {
  std::ifstream file(_cache_file, std::ifstream::in);
  while(!file.eof()) {
    DiscoveryClient c;
    c.read(file);
    _clients[c.serial] = c;
  }
  file.close();
}
