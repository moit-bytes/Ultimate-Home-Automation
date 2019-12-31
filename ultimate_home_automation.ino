 /***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  Code is edited by Sachin Soni for it's project called
  Ultimate Home Automation

  For Project video, visit his YouTube channel named "techiesms"

  Website:- http://www.techiesms.com
  
  #techiesms
  explore | learn | share
  
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"






/************************* Pin Definition *********************************/

//Relays for switching appliances
#define Relay1 D6
#define Relay2 D2
#define Relay3 D1
#define Relay4 D5



/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Your SSID"
#define WLAN_PASS       "Your password"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Username123"//"Your Username"
#define AIO_KEY         "Your AIO key"//"912b30c900574034a653f41e2b4df838"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/




// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light-1");
Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light-2");
Adafruit_MQTT_Subscribe light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light-3");
Adafruit_MQTT_Subscribe light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/light-4");




void MQTT_connect();

void setup() {
  Serial.begin(115200);

  delay(10);

  
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
 
void MQTT_connect();
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&light1);
  mqtt.subscribe(&light2);
  mqtt.subscribe(&light3);
  mqtt.subscribe(&light4);
}

uint32_t x = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)light1.lastread);
      int light1_State = atoi((char *)light1.lastread);
      digitalWrite(Relay1, light1_State);
      
    }
    if (subscription == &light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)light2.lastread);
      int light2_State = atoi((char *)light2.lastread);
      digitalWrite(Relay2, light2_State);
    }
    if (subscription == &light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)light3.lastread);
      int light3_State = atoi((char *)light3.lastread);
      digitalWrite(Relay3, light3_State);
    }
    if (subscription == &light4) {
      Serial.print(F("Got: "));
      Serial.println((char *)light4.lastread);
      int light4_State = atoi((char *)light4.lastread);
      digitalWrite(Relay4, light4_State);
      
    }
  }
}
 
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
