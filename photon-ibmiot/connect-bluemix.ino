#include "MQTT/MQTT.h"

char *IOT_CLIENT = "d:org_id:device_type:device_id";
char *IOT_HOST = "org_id.messaging.internetofthings.ibmcloud.com";
char *IOT_PASSWORD = "_device_token_here_";
char *IOT_PUBLISH = "iot-2/evt/randomnumber/fmt/json";
char *IOT_SUBSCRIBE = "iot-2/cmd/alert/fmt/json";
char *IOT_USERNAME = "use-token-auth";

int count = 1;
int led1 = D0;
int led2 = D7;

void callback( char* topic, byte* payload, unsigned int length ) {
  payload[length] = '\0';
  String s = String((char*)payload);
  int f = s.toInt();
  //Particle.publish("payload", String(s), PRIVATE);
  if (f == 1){
     digitalWrite(led1, HIGH);
     digitalWrite(led2, HIGH);
     delay(100);
     digitalWrite(led1, LOW);
     digitalWrite(led2, LOW);
  }
}

MQTT client(IOT_HOST, 1883, callback);

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  client.connect(
    IOT_CLIENT,
    IOT_USERNAME,
    IOT_PASSWORD
  );
  //client.subscribe( IOT_SUBSCRIBE );
  if (client.subscribe(IOT_SUBSCRIBE)) {
      Particle.publish("subscribe", String("subscribe OK"), PRIVATE);
    } else {
      Particle.publish("subscribe", String("subscribe failed"), PRIVATE);
    }

  if( client.isConnected() ) {
      Particle.publish("status", String("Connected"), PRIVATE);
    } else {
      Particle.publish("status", String("Unconnected"), PRIVATE);
    }
}

void loop() {
    client.publish(IOT_PUBLISH, "{\"value\": " + String(random(100)) + " }"
    );
    //Particle.publish("counting", String(count), PRIVATE);
    count = count + 1;
    client.loop();
    delay( 1000 );
}
