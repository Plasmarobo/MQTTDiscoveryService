#ifndef MQTT_DISCOVERY_SERVER_H
#define MQTT_DISCOVERY_SERVER_H

#include <mosquittopp.h>
#include <string>
#include <map>

class MQTTDiscoveryServer : public mosqpp::mosquittopp {
public:
  
  struct DiscoveryClient {
    std::string category;
    std::string name;
    uint32_t enumerator;
    uint32_t serial;

    void read(std::ifstream &file) {
      file >> name;
      file >> serial;
      file >> enumerator;
      file >> category;
    }
    void write(std::ofstream &file) {
      file << name << endl;
      file << serial << endl;
      file << enumerator << endl;
      file << category << endl;
    }
  };
  MQTTDiscoveryServer(const char* config_file,
                      const char* cache_file,
                      const char* host,
                      int port=1883);
  ~MQTTDiscoveryServer();

  void on_connect(int rc);
  void on_message(const struct mosquitto_message *message);
  void on_subscribe(int mid, int qos_count, cont int *granted_qos);
  void add_client(std::string discovery_name, std::string category, std::string client_name);

protected:
  void write_config();
  void write_cache();
  void read_config();
  void read_cache();

  std::string _config_file;
  std::string _cache_file;
  uint32_t _serial_index;
  std::list<std::string> _pending;
  std::map<uint32_t, MQTTDiscoveryServer::DiscoveryClient> _clients;
  const int32_t KEEPALIVE_SEC = 60;
};
#endif /* MQTT_DISCOVERY_SERVER_H */
