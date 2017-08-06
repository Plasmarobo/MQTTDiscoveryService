# MQTT Discovering and naming protocol
An MQTT Service which provides naming and enumeration services.
Like MQTT itself is uses Client-names and topics.
## Hierarchy
-Topic
   -Name
     -Enumeration (numbering/positioning)

## Discovery Protocal
The Discovery functions on a client server model.
Both communicate via MQTT topics. Specifically the top-level topic /discover
A client will post a temporary name (given by a fixed prefix implemented in the discovery-client code). This same prefix must be used to instantiate a server. A large (32-bit) random number is generated. Since this is a temporary measure, 32-bit is sufficient to avoid collision (we expect only 1 client at a time, but should be able to handle ~100 without significant statistical collision). There is no anti collision protocol, but if MQTT client name collision occurs after assigment clients will revert to their discovery name.
The Discovery-server constantly listens on the /discover channel. Once any client reports it's name, the Discovery-server generates a unique serial number (from a set of internal prefixes and incremental id column) and assigns in to the client. It also generates a human-readable name from the Name-Enumerator combinaton provided by the users (Lights-01 or LivingRoomLights). The serial number should be saved in persistant storage on the client. It will require reconfiguring to assign it again. The human-readable name is saved in a table server side. It can be regenerated from the serial number.

## Tests
Name Generation
Client Communication
Serial Number Generation
HRN Generation
Server Communication
Client Persistance
Server-Side HRN Regeneration
Full-Stack Operation   

## UI
A convenient web UI on port 7533 is made available for configuration and administration
