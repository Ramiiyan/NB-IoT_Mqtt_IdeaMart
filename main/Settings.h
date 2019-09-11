#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// change these topics and mqtt details with your Ideamart developer portal.
#define RES_TOPIC  "/2952727675078424/tester/testing/v1/sub" 
#define SUB_TOPIC  "+/2952727675078424/tester/testing/v1/sub"
#define PUB_TOPIC  "tester/testing/v1/common"

#define MQTT_USERNAME "tester-testing-v1_2589"
#define MQTT_PASSWORD "1563971290_2589"
#define MQTT_HOST "mqtt.iot.ideamart.io"
#define MQTT_PORT 1883

#define ideaBoard_PWRKEY 13
#define ideaBoard_RX 8
#define ideaBoard_TX 7
#define ideaBoard_RST 11

SoftwareSerial SerialSIM(7, 8);
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

String clientId = "";
char getmsg[100];
char setmsg[100];
String on = "{\"action\":\"on\",\"param\":{\"mac\":\"2952727675078424\"}}"; //change with your mac address
String off = "{\"action\":\"off\",\"param\":{\"mac\":\"2952727675078424\"}}";
String cpsi;
boolean ConnectToMQTT();
void sendVal(int cpsi);
void CallBack(char *t, byte *payload, int l);
String getCPSI();
int pin = 9;

boolean ConnectToMQTT(){
  randomSeed(analogRead(5)); //analog pin 5 used to genarate random number
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
  Serial.println(F("******************"));
  for (int i = 0; i < l; i++){
    getmsg[i] = (char)payload[i];
  }
  if (on == getmsg){
    Serial.println(F("Action : ON"));
    digitalWrite(pin, HIGH);
  }else if (off == getmsg){
    Serial.println(F("Action : OFF"));
    digitalWrite(pin, LOW);
  }
  delay(1000);
  //memset(getmsg, 0, sizeof(getmsg));
}

void sendVal(int val){
  sprintf(setmsg,"{\"eventName\":\"Tester\",\"status\":\"\",\"value\":\"%ul\",\"param\":{\"mac\":\"2952727675078424\"}}",millis()); //change your mac address
  if (!mqtt.publish(PUB_TOPIC, setmsg)){
    Serial.println(F("Failed."));
  }else{
    Serial.print(setmsg);
    Serial.println(F("SEND:OK"));
  }
  delay(1000);
}

String getCPSI(){
  SerialSIM.println("AT+CPSI?");
  delay(500);
  while (SerialSIM.available()){
    cpsi = SerialSIM.readString();
  }
  delay(500);
  return cpsi;
}
