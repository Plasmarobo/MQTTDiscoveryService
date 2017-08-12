# MQTT Discovering and naming protocol
An MQTT Service which provides naming and enumeration services.
Like MQTT itself is uses Client-names and topics.
## Hierarchy
*Topic
  *Name
    *Enumeration (numbering/positioning)

## Discovery Protocal
The Discovery functions on a client server model.
Both communicate via MQTT topics. Specifically the top-level topic /discover
A client will post a temporary name (given by a fixed prefix implemented in the discovery-client code). This same prefix must be used to instantiate a server. A 32-bit random number is generated. Since this is a temporary measure, 32-bit is sufficient to avoid collision (we expect only 1 client at a time, but should be able to handle ~100 without significant statistical collision). There is no anti collision protocol, but if MQTT client name collision occurs after assigment clients will revert to their discovery name.
The Discovery-server constantly listens on the /discover channel. Once any client reports it's name, the Discovery-server generates a unique serial number (from a set of internal prefixes and incremental id column) and assigns in to the client. It also generates a human-readable name from the Name-Enumerator combinaton provided by the users (Lights-01 or LivingRoomLights). The serial number should be saved in persistant storage on the client. It will require reconfiguring to assign it again. The human-readable name is saved in a table server side. It can be regenerated from the serial number. Serial number 0 is considered invalid/unconfigured. Note the "discovery" field should match the MQTT client name.

The discovery messages are simple json messages. An object is posted to /discover
An example transaction:
**Client:**
```{"discovery": "Lights-Smartbulb-000000FF","request":"register"}```
**Server:**
```{"client":"Lights-Smartbulb-000000FF", "serial":1000000100250099, "name": "LivingRoomLight", "enum":1, "request": "initialization"}```
Optional confirmation (server doest not respond to a confirmation, but may use it as a success condition)
**Client:**
```{"client":"LivingRoomLight", "enum":01, "serial":1000000100250099, "request":"state"}```

A name query is also accepted by the server:
**Client**
```{"serial":1000000100250099, "request": "name"}```
**Server**
```{"serial":1000000100250099, "name": "LivingRoomLight", "enum":1}```
**Client**
```{"client":"LivingRoomLight", "enum":01, "serial":1000000100250099, "request":"state"}```

## Tests
Name Generation
Client Communication
Serial Number Generation
Server Communication
Client Persistance
Server-Side HRN Regeneration

## UI
A web UI is in progress
