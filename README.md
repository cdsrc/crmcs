# Cloud based Real-time Monitoring and Control System
This project uses cloud platform to conduct process monitoring and control, e.g.,
additive manufacturing process monitoring and control.

## 1. Connect multiple sensors with Arduino

## 2. Connect Photon with Arduino

## 3. Connect Photon with IBM IoT
The connection is via MQTT.

Current codes are:
```cpp
#include "MQTT/MQTT.h"

char *IOT_CLIENT = "d:org_ID:device_type:device_ID";
char *IOT_HOST = "org_ID.messaging.internetofthings.ibmcloud.com";
char *IOT_PASSWORD = "_device_token_here_";
char *IOT_PUBLISH = "iot-2/evt/randomnumber/fmt/json";
char *IOT_USERNAME = "use-token-auth";

int count = 1;

MQTT client(IOT_HOST, 1883, callback);

void setup() {

  client.connect(
    IOT_CLIENT,
    IOT_USERNAME,
    IOT_PASSWORD
  );

  if( client.isConnected() ) {
    Serial.println( "Connected." );
    Particle.publish("status", String("Connected"), PRIVATE);
    // client.subscribe( IOT_SUBSCRIBE );
  } else {
      Particle.publish("status", String("Unconnected"), PRIVATE);
  }
}
void loop() {
    client.publish(IOT_PUBLISH, "{\"value\": " + String(random(100)) + " }"
    );

    Particle.publish("counting", String(count), PRIVATE);
    count = count + 1;
    delay( 10000 );
    client.loop();
}

void callback( char* topic, byte* payload, unsigned int length ) {
  char p[length + 1];

  memcpy( p, payload, length );
  p[length] = NULL;

  String message( p );
}

```

## 4. Store and analyze data within IBM cloud

## 5. Send feedback (analysis results) to Photon/Arduino
