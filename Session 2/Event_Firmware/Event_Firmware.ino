/* *********Session 2**************
 * This code is for Event Configuration.
 * It's only Publishing the Button State to Developer Portal. 
 * Upload this code to DevBoard that you Connected the Push-button.
 * PushButton Connected pin - 5
 * 
 * DEFALUT Configuration for GSM
 * 
 * Ideamart DevBoard Available @ - https://go.ideamart.io/nbiotdevboard
 */
#include "Settings.h"

int ButtonState = 1;
int oldButtonState;
String clientId = "";
char setmsg[100];
int buttonPin = 5;       // for Button Input
String cpsi;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin,INPUT_PULLUP);
  
  pinMode(ideaBoard_PWRKEY, OUTPUT);
  digitalWrite(ideaBoard_PWRKEY, HIGH);
  delay(10);

  SerialSIM.begin(4800);
  modem.setNetworkMode(13); // 38-nbiot 13-gsm
  //modem.setPreferredMode(2);  //<-- Uncomment this, if you are using nbiot,.
  modem.getModemName();

  modem.restart();
  delay(1000);
  Serial.println(F("Finding Network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(F("Network Fail"));  // try to restart the Ideamart DevBoard. or Reupload the code. check whether Sim Install properly.
    while (true);
  }else {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect(GSM_APN, "", "")) {      //NB_APN for nbiot
      Serial.println(F("GPRS Fail"));  //restart the devBoard. 
    }else {
      Serial.println(F("GPRS Connected."));
      Serial.println(getCPSI());
      mqtt.setServer(MQTT_HOST, MQTT_PORT);
    }
  }
}

void loop() {
  
  if (mqtt.connected()) {
    mqtt.loop();
  }else {
    Serial.println(F("Connecting MQTT..."));
    digitalWrite(ideaBoard_PWRKEY, HIGH);
    delay(180);
    digitalWrite(ideaBoard_PWRKEY, LOW);
    delay(500);
    ConnectToMQTT();
    delay(10);
  }
  Serial.print(F("ButtonState : "));
  Serial.println(ButtonState);
  if(oldButtonState!=ButtonState) {
    sendVal(ButtonState);
    oldButtonState = ButtonState;
  }else {
     ButtonState = digitalRead(buttonPin);
  }
  delay(250); // every 0.25 second
}

// connecting to mqtt
boolean ConnectToMQTT() {
  randomSeed(analogRead(5));    //analog pin 5 used to genarate random number
  clientId = "TEST-" + String(millis()) + String(random(0, 100000000), HEX);
  Serial.print(F("Client ID :"));
  Serial.println(clientId);
  if (!mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println(F("MQTT fail"));
    return false;
  }
  Serial.println(F("MQTT Connected."));
  return mqtt.connected();
}

// Publishing the Event as Json Object
void sendVal(int val) {
  sprintf(setmsg,"{\"eventName\":\"XXXXXXXX\",\"status\":\"none\",\"value\":\"%d\",\"param\":{\"mac\":\"XXXXXXXXXXXXXXXX\"}}",val); //change your mac address
  if (!mqtt.publish(PUB_TOPIC, setmsg)){
    Serial.println(F("Publish Event Failed."));
  }else{
    Serial.println(F("Event Sent."));
    Serial.print(F("Published Message :"));
    Serial.println(setmsg);
  }
  delay(100);
}

// getting CPSI of the Sim
String getCPSI() {
  SerialSIM.println("AT+CPSI?");
  delay(500);
  while (SerialSIM.available()) {
    cpsi = SerialSIM.readString();
  }
  delay(500);
  return cpsi;
}
