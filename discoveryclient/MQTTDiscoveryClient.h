//Arduino MQTTDiscoveryClient
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJSON.h>

typedef uint32_t serial_t;
#define WRITE_BYTES(start, count, source) for(uint32_t i=0;i<count;++i) EEPROM.write(start+i, source>>(8*i)))

class MQTTDiscoveryClient {
public:
  MQTTDiscoveryClient(const char* category, const char* name) {
    sprintf(&_name,"%s-%s-%x", category, name, rand());
    _serial = 0;
    EEPROM.begin(512);
    //Serial Number is 4 bytes
    readSerial();
  }

  void DiscoverQuery(PubSubClient &mqtt) {
    if (_serial == 0) {
      char buffer[256];
      sprintf(&buffer[0],
        "{\"discovery\": \"%s\",\"request\":\"register\"}", _name);
      mqtt.publish(_DISCOVER_TOPIC, &buffer[0]);        
    }
  }

  void NameQuery(PubSubClient &mqtt) {
    if (_serial == 0) {
      char buffer[256];
      sprintf(&buffer[0], 
        "{\"serial\":%d, \"request\":\"name\"}", 
        _serial);
      mqtt.publish(_DISCOVER_TOPIC, &buffer[0]);
    }
  }

  void MQTTCallback(const char* topic, const uint8_t* payload, const uint32_t &len) {
    if (strcmp(topic, _DISCOVER_TOPIC) != 0)
      return;
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject((const char*) payload);
    if (!root.success()) {
      return;
    }
    //Handle Discovery Query
    if(root.containsKey("client") && 
       (strcmp(root["client"], _name) == 0) &&
       root.containsKey("serial")) {
      _serial = root["serial"];
      writeSerial();
      writeName(root["name"], root["enum"]);
    }
    //Handle Name Query
    if(root.containsKey("serial") &&
       atoi(root["serial"] == _serial) &&
       root.containsKey["name"] &&
       root.ctonainsKey["enum"]) {
         writeName(root["name"], root["enum"]);
    }
  }
protected:
  void readSerial() {
    uint8_t buffer;
    for(uint8_t addr = 0; addr < SERIAL_BYTES; ++addr) {
      EEPROM.read(0, buffer);
      _serial |= buffer << (8 * addr);
    }
  }
  void writeSerial(uin32_t serial) {
    WRITE_BYTES(0, SERIAL_BYTES, serial);
  }
  void writeName(const char* name, uint32_t enumerator) {
    char buffer[NAME_BYTES];
    sprintf(buffer, "%s-%d", name, enumerator);
    uint16_t len = strlen(buffer);
    WRITE_BYTES(SERIAL_BYTES, LEN_BYTES, len);
    for(uint32_t i = 0; i < len; ++i) {
      EEPROM.write(i + SERIAL_BYTES + LEN_BYTES, buffer[i]);
    }
  }
  const uint32_t EEPROM_BYTES = 512;
  const uint32_t SERIAL_BYTES = 4;
  const uint32_t LEN_BYTES = 2;
  const uint32_t NAME_BYTES = EEPROM_BYTES - (SERIAL_BYTES + LEN_BYTES);

  char _name[NAME_BYTES];
  serial_t _serial;
  const char* _DISCOVER_TOPIC = "/discover";
};
