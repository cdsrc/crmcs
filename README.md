# Cloud based Real-time Monitoring and Control System (ongoing)
This project uses cloud platform (IBM Cloud) to conduct process monitoring and control, e.g.,
additive manufacturing process monitoring and control: ![alt text](https://raw.githubusercontent.com/cdsrc/crmcs/master/assets/images/connected-AM.png).

The system's hardware include: sensors (gyroscope, camera etc.), Arduino, and Photon.
To build the system, there are two major steps, i.e., (1) enable the connection of sensors-Arduino-Photon-IBMCloud;
(b) create data analysis models and algorithms to analyze the input sensors data, and give output of monitoring results and control signals.

## 1. Build the connection of sensors-Arduino-Photon-IBMCloud
The connection includes: (a) connect multiple sensors with Arduino; (b) communications of Arduino-Photon
(c) communications of Photon-IBMCloud.
### 1.1 Connect multiple sensors with Arduino
### 1.2 Communications of Arduino-Photon
### 1.3 Communications of Photon-IBMCloud
* Set up IBM Watson IoT, and add device to the IoT platform. An instruction can be found [here](https://www.kevinhoyt.com/2016/04/27/particle-photon-on-watson-iot/).
* Write codes to Photon to enable the connection. The connection is via MQTT. A simple example that sends and analyzes random number is as follows:
  * generate 0-100 random numbers and send the number to IBM IoT (`client.connect` and `client.publish`);
  * use **Node-RED** to simply analyze the random number, i.e., if the number is bigger than a specified threshold,
  then return "1", otherwise, return "0". The returned commands "1" or "0" is sent back to IBM Cloud. The Node-RED flows can be found [here](http://node-red-ammonitoring.mybluemix.net/red/);
  * receive the "1" or "0" commands from IBM Cloud to Photon (`client.subscribe`), and blink the LED lights if the command is "1".  
* The codes are as follows:

```cpp
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
```
* Notes: If the connection to IBM IoT failed, try modifying the security settings of IBM IoT:
in "SECURITY-Connection Security", change the "security level" to "TLS Optional". Also, the Node-RED application must be built, otherwise, no feedback command is generated.

## 2. Store and analyze data using IBM cloud
* A cloud database storing historical data must be built;
* Algorithms analyzing streaming data and historical data should be proposed based on the specific monitoring/control task. (**Streaming Analytics** in IBM Cloud, and **Node-RED** are potentially to be used.)

## Acknowledgment
This project is supported by the National Science Foundation, RISE NSF#1646897.
