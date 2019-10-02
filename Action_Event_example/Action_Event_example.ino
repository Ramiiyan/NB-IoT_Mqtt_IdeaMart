/* ********* EXAMPLE **************
 *   
 * You Can Publish the Button State To the Developer Portal and 
 * Subscribe the Action From the Developer Portal to turn ON/OFF the LED.
 * 
 * Default Configuration for GSM
 * Ideamart DevBoard Available @ - https://go.ideamart.io/nbiotdevboard
 */

#include "Settings.h"

// Examples.. for Action
//  String on = "{\"action\":\"on\",\"param\":{\"mac\":\"2951212455078424\"}}"; 
//  String off = "{\"action\":\"off\",\"param\":{\"mac\":\"2951212455078424\"}}";

// Replace the XXXX with your developer credentials
String on = "{\"action\":\"XXX\",\"param\":{\"mac\":\"XXXXXXXXXXXXXXXXX\"}}"; 
String off = "{\"action\":\"XXX\",\"param\":{\"mac\":\"XXXXXXXXXXXXXXXXX\"}}";

String clientId = "";
char getmsg[100];
char setmsg[100];
String cpsi;
int ledPin      = 4;                // for LED output
int buttonPin   = 5;                // for Button Input
int buttonState = 1;
int oldButtonState;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);                 //led output pin -4
  pinMode(buttonPin,INPUT_PULLUP);         //push button Input pin -5
  
  pinMode(ideaBoard_PWRKEY, OUTPUT);
  digitalWrite(ideaBoard_PWRKEY, HIGH);
  delay(10);


  SerialSIM.begin(4800);
  modem.setNetworkMode(13);             // 38-nbiot 13-gsm
  //modem.setPreferredMode(2);          //<-- Uncomment this, if you are using nbiot,.
  modem.getModemName();
  
  modem.restart();
  delay(1000);
  Serial.println(F("Finding Network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(F("Network Fail"));
    while (true);
  }else {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect(GSM_APN, "", "")) {  //change the APN to NB_APN for nbiot 
      Serial.println(F("GPRS Fail"));  
    }else {
      Serial.println(F("GPRS Connected."));
      Serial.println(getCPSI());
      mqtt.setServer(MQTT_HOST, MQTT_PORT);
      mqtt.setCallback(CallBack);
    }
  }
}

void loop(){
  
  if (mqtt.connected()) {
    mqtt.loop();
  }else {
    Serial.println(F("Connecting MQTT..."));
    digitalWrite(ideaBoard_PWRKEY, HIGH);
    delay(180);
    digitalWrite(ideaBoard_PWRKEY, LOW);
    delay(1000);
    ConnectToMQTT();
    delay(100);
  }
  Serial.print(F("ButtonState :"));
  //int ButtonState = digitalRead(button);
  Serial.println(buttonState);

  if(oldButtonState!=buttonState) {
    sendVal(buttonState);
    oldButtonState = buttonState;
  }else {
     buttonState = digitalRead(buttonPin);
  }
  delay(500); // delay for 0.5 second.
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

void CallBack(char *t, byte *payload, int l) {
  Serial.println(F("Action message is Received."));
  
  for (int i = 0; i < l; i++){
    getmsg[i] = (char)payload[i];
  }
  
  Serial.print(F("Message : "));
  Serial.println(getmsg);
  
  if (on == getmsg) {
    Serial.println(F("Action : ON"));
    digitalWrite(ledPin, HIGH);
  }else if (off == getmsg) {
    Serial.println(F("Action : OFF"));
    digitalWrite(ledPin, LOW);
  }
  
  delay(1000);
  memset(getmsg, 0, sizeof(getmsg));
  
}

void sendVal(int val) {
  sprintf(setmsg,"{\"eventName\":\"XXXXXXXX\",\"status\":\"none\",\"value\":\"%d\",\"param\":{\"mac\":\"XXXXXXXXXXXXXXXX\"}}",val); //change your mac address
  if (!mqtt.publish(PUB_TOPIC, setmsg)) {
    Serial.println(F("Publish Event Failed."));
  }else {
    Serial.println(F("Event Sent."));
    Serial.print(F("Published Message :"));
    Serial.println(setmsg);
  }
  delay(100);
}

String getCPSI() {
  
  SerialSIM.println("AT+CPSI?");
  delay(500);
  while (SerialSIM.available()) {
    cpsi = SerialSIM.readString();
  }
  delay(500);
  return cpsi;

}
