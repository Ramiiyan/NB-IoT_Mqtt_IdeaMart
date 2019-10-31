#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// change these topics and mqtt details with your Ideamart developer portal.

//  Examples for Topic...
//  #define SUB_TOPIC       "+/2951212455078424/testing/tester/v1/sub"

//  Examples for Mqtt username, password
//  #define MQTT_USERNAME   "testing-tester-v1_9876"
//  #define MQTT_PASSWORD   "108644667_9876"

// Replace the XXXX with your developer credentials
#define SUB_TOPIC           "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" //Subscription Topic

#define MQTT_USERNAME       "XXXXXXXXXXXXXXXXXXXXXXX"
#define MQTT_PASSWORD       "XXXXXXXXXXXXXXXXX"

// No need to change the mqtt host and port util you use Ideamart-IoT web portal.
#define MQTT_HOST           "mqtt.iot.ideamart.io" 
#define MQTT_PORT           1883

#define ideaBoard_PWRKEY    13
#define ideaBoard_RX        8
#define ideaBoard_TX        7
#define ideaBoard_RST       11
#define GSM_APN             "dialogbb"
#define NB_APN              "nbiot"

SoftwareSerial SerialSIM(7, 8);
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

//functions
boolean ConnectToMQTT();
void CallBack(char *t, byte *payload, int l);
String getCPSI();
