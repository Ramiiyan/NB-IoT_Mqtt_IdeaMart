/* ********* Session 1 **************
 *  
 * This code is for Action Configuration ( Light ON/OFF ).
 * It's only Subscribing the Action From the Developer Portal.
 * Go to Settings tab to change the Topic and mqtt crendentials.
 * Upload this code to DevBoard that you Connected the LED.
 * LED conected pin - 4
 * 
 * DEFALUT Configuration for GSM
 * Ideamart DevBoard Available @ - https://go.ideamart.io/nbiotdevboard
 */

#include "Settings.h"

//change your Action Values and mac address.

//Example
//  String on   = "{\"action\":\"on\",\"param\":{\"mac\":\"2951212455078424\"}}"; 
//  String off  = "{\"action\":\"off\",\"param\":{\"mac\":\"2951212455078424\"}}";

String on       = "{\"action\":\"on\",\"param\":{\"mac\":\"XXXXXXXXXXXXXXXXX\"}}"; 
String off      = "{\"action\":\"off\",\"param\":{\"mac\":\"XXXXXXXXXXXXXXXX\"}}";

String clientId = "";
char getmsg[100];
String cpsi;
int ledPin      = 4;                          // for LED output

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);         //LED output pin - 4
  
  pinMode(ideaBoard_PWRKEY, OUTPUT);
  digitalWrite(ideaBoard_PWRKEY, HIGH);
  delay(10);
  Serial.print("IdeaBoard initializing...");
  SerialSIM.begin(4800);
  modem.setNetworkMode(38); // 38-nbiot 13-gsm
  modem.setPreferredMode(2);  //<-- Uncomment this, if you are using nbiot(nbiot),. comment this if you are using dialogbb(gsm)
  modem.getModemName();

  modem.restart();
  delay(2000);
  Serial.println(F("Finding Network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(F("Network Fail"));  // try to restart the Ideamart DevBoard. or Reupload the code. check whether Sim Install properly.
    while (true);
  }else {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());

    if (!modem.gprsConnect(NB_APN, "", "")) {  //NB_APN for nbiot || GSM_APN for gsm
      Serial.println(F("GPRS Fail"));
    }else {
      Serial.println(F("GPRS Connected"));
      Serial.println(getCPSI());
      mqtt.setServer(MQTT_HOST, MQTT_PORT);
      mqtt.setCallback(CallBack);
    }
  }
}

void loop() {

  if (mqtt.connected()) {
    mqtt.loop();
  }else {
    Serial.println("Connecting MQTT...");
    digitalWrite(ideaBoard_PWRKEY, HIGH);
    delay(180);
    digitalWrite(ideaBoard_PWRKEY, LOW);
    delay(1000);
    ConnectToMQTT();
    delay(100);
  }

}

// Connecting to MQTT
boolean ConnectToMQTT(){
  randomSeed(analogRead(5));          //analog pin 5 used to genarate random number
  clientId = "TEST-" + String(millis()) + String(random(0, 100000000), HEX);
  Serial.print(F("ClientID :"));
  Serial.println(clientId);

  if (!mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println(F("MQTT fail"));
    return false;
  }
  Serial.println(F("MQTT Connected."));
  mqtt.subscribe(SUB_TOPIC);
  Serial.print(F("Subscription Topic :"));
  Serial.println(SUB_TOPIC);
  return mqtt.connected();
}

// CallBack Function for Receive Msg as Json Object.
void CallBack(char *t, byte *payload, int l){
  Serial.println(F("Action message is Received."));
  for (int i = 0; i < l; i++){
    getmsg[i] = (char)payload[i];
  }

  Serial.print(F("Message : "));
  Serial.println(getmsg);

  if (on == getmsg){
    Serial.println(F("Action : ON"));
    digitalWrite(ledPin, HIGH);
  }else if (off == getmsg){
    Serial.println(F("Action : OFF"));
    digitalWrite(ledPin, LOW);
  }

  delay(500);
  memset(getmsg, 0, sizeof(getmsg));

}

// CPSI of the Sim
String getCPSI(){
  SerialSIM.println("AT+CPSI?");
  delay(500);
  while (SerialSIM.available()){
    cpsi = SerialSIM.readString();
  }
  delay(500);
  return cpsi;
}
