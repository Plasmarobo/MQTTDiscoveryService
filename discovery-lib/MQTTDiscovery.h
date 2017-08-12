#typedef serial_t uint32_t;

class MQTTAttributes {
    public:
        MQTTAttributes(std::string category, std::string name) {
            _category = category;
            _name = _name;
            _enumeration = rand();
        }
    protected:
        std::string _category;
        std::string _name;
        uint32_t _enumeration;
};

class MQTTDiscoveryServer : public MQTTAttributes {
    public:
        MQTTDiscoveryServer(std::string config_file);


    protected:
        void saveConfig();
        void loadConfig();

        std::string _config_file;
        std::map<std::string, std::string> _names;
        std::map<serial_t, std::string> _clients;
};

class MQTTDiscoveryClient : public MQTTAttributes {
    public:
        MQTTDiscoveryClient();
    protected:
        virtual void writeSerial();
        virtual void loadSerial();

        std::string _category;
        serial_t _serial_no;
};