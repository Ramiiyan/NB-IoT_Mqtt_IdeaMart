#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// change these topics and mqtt details with your Ideamart developer portal.

//  Examples for Topic...
//  #define SUB_TOPIC       "+/2951212455078424/testing/tester/v1/sub"
//  #define RES_TOPIC       "/2951212455078424/testing/tester/v1/sub"
//  #define PUB_TOPIC       "testing/tester/v1/common"

//  Examples for Mqtt username, password
//  #define MQTT_USERNAME   "testing-tester-v1_9876"
//  #define MQTT_PASSWORD   "108644667_9876"

// Replace the XXXX with your developer credentials
#define SUB_TOPIC           "+/2952727675078424/tester/testing/v1/sub" //Subscription Topic
#define RES_TOPIC           "2952727675078424/tester/testing/v1/pub"   //Subscription Respose Topic
#define PUB_TOPIC           "tester/testing/v1/common"                 //Publishing Topic

#define MQTT_USERNAME       "tester-testing-v1_2589"
#define MQTT_PASSWORD       "1563971290_2589"

// No need to change the mqtt host and port util you use Ideamart-IoT web portal.
#define MQTT_HOST           "mqtt.iot.ideamart.io" 
#define MQTT_PORT           1883

#define ideaBoard_PWRKEY    13
#define ideaBoard_RX        8
#define ideaBoard_TX        7
#define ideaBoard_RST       11
#define GSM_APN             "dialogbb"
#define NB_APN              "nbiot"

SoftwareSerial SerialSIM(ideaBoard_TX, ideaBoard_RX);
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

boolean ConnectToMQTT();
void sendVal(int val);
void CallBack(char *t, byte *payload, int l);
String getCPSI();

// Examples.. for Action
//  String on = "{\"action\":\"on\",\"param\":{\"mac\":\"2951212455078424\"}}"; 
//  String off = "{\"action\":\"off\",\"param\":{\"mac\":\"2951212455078424\"}}";

// Replace the XXXX with your developer credentials
String on = "{\"action\":\"on\",\"param\":{\"mac\":\"2952727675078424\"}}"; 
String off = "{\"action\":\"off\",\"param\":{\"mac\":\"2952727675078424\"}}";

String clientId = "";
char getmsg[100];
char setmsg[100];
String cpsi;
int pin         = 4;                // for LED output
int button      = 5;                // for Button Input
int ButtonState = 1;
int oldButtonState;

boolean ConnectToMQTT() {
  
  randomSeed(analogRead(5));          // analog pin 5 used to genarate random number
  clientId = "TEST-" + String(millis()) + String(random(0, 100000000), HEX);
  Serial.println(clientId);
  if (!mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
  {
    Serial.println("MQTT fail");
    return false;
  }
  Serial.println(SUB_TOPIC);
  mqtt.subscribe(SUB_TOPIC);
  return mqtt.connected();
}

void CallBack(char *t, byte *payload, int l){
  Serial.println(F("Action message is Received."));
  
  for (int i = 0; i < l; i++){
    getmsg[i] = (char)payload[i];
  }
  
  Serial.print(F("Message : "));
  Serial.println(getmsg);
  
  if (on == getmsg) {
    Serial.println(F("Action : ON"));
    digitalWrite(pin, HIGH);
  }else if (off == getmsg) {
    Serial.println(F("Action : OFF"));
    digitalWrite(pin, LOW);
  }
  
  delay(1000);
  memset(getmsg, 0, sizeof(getmsg));
  
}

void sendVal(int val) {
  
  // Replace the XXXX with your developer credentials
  sprintf(setmsg,"{\"eventName\":\"Tester\",\"status\":\"none\",\"value\":\"%d\",\"param\":{\"mac\":\"2952727675078424\"}}",val); //change your mac address
  
  if (!mqtt.publish(PUB_TOPIC, setmsg)) {
    Serial.println(F("Failed."));
  }else{
    Serial.print(setmsg);
    Serial.println(F("SEND:OK"));
  }
  delay(1000);
  
}

String getCPSI() {
  
  SerialSIM.println("AT+CPSI?");
  delay(500);
  while (SerialSIM.available()){
    cpsi = SerialSIM.readString();
  }
  delay(500);
  return cpsi;

}
