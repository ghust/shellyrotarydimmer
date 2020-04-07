
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

#include "arduino_secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

const char* mqtt_server = SECRET_MQTT_IP;
const int   mqtt_port = SECRET_MQTT_PORT;
const char* mqtt_user = SECRET_MQTT_USER;
const char* mqtt_password = SECRET_MQTT_PASSWD;


const char* mqtt_chan = "shellies/shellydimmer-<deviceid>/light/0/set"; //change to your deviceid.
const char* mqtt_debug = "somedebugchan";




unsigned long ts = 0, new_ts = 0; //timestamp for loop

byte pin = A0; //A0 pin on the Wemos D1
int mqtt_val = 0; //int used to peristently store the read potentiometer value.

//use this to send MQTT messages.
void publishMQTT(String topic, String incoming) {
  Serial.println("MQTT: " + topic + ":" + incoming);
  char charBuf[incoming.length() + 1];
  incoming.toCharArray(charBuf, incoming.length() + 1);

  char topicBuf[topic.length() + 1];
  topic.toCharArray(topicBuf, topic.length() + 1);
  client.publish(topicBuf, charBuf);
}

//use this to send the value.
void publishvalMQTT(String topic, String incoming) {
    String command = "on";
    if(incoming == "0"){command = "off";}
    String payload = "{\"brightness\":"+incoming+",\"turn\":"+command+"}";
    incoming = payload;
  Serial.println("MQTT: " + topic + ":" + incoming);
  char charBuf[incoming.length() + 1];
  incoming.toCharArray(charBuf, incoming.length() + 1);

  char topicBuf[topic.length() + 1];
  topic.toCharArray(topicBuf, topic.length() + 1);
  client.publish(topicBuf, charBuf);
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect using a random clientid
    String clientId = "Thermostat-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected as " + clientId);
      // Once connected, publish an announcement...
      publishMQTT(mqtt_debug,"ping from potentiometer");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  //Connect to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ts = millis();
  setup_wifi();
  
    //Init MQTT Client
  client.setServer(mqtt_server, mqtt_port);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  new_ts = millis(); //current timestamp
  if (new_ts - ts > 100) { //if 100ms has elapsed
    ts = millis(); //reset timer

    int val = analogRead(pin); // reads potentiometer value (between 0 and 1023)
    val = map(val, 0, 1023, 0, 100); // scale it to use it with the servo (between 0 and 100)

    if(val != mqtt_val){
      mqtt_val = val;
      publishvalMQTT(mqtt_chan,String(mqtt_val));
      }
  }

  //MQTT Loop
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
