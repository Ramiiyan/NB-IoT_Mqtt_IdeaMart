/* ********* Session 1 **************
 *  
 * This code is Build For Session 1.
 * You will learn about how the Dialog Ideamart Developer Portal works. 
 * You Can Publish the Button State To the Developer Portal and 
 * Subscribe the Action From the Developer Portal to turn ON/OFF the LED.
 * 
 * Default Configuration for GSM
 */

#include "Settings.h"

void setup() {
  Serial.begin(115200);
  pinMode(pin, OUTPUT);                 //led output pin -4
  pinMode(button,INPUT_PULLUP);         //push button Input pin -5
  
  pinMode(ideaBoard_PWRKEY, OUTPUT);
  digitalWrite(ideaBoard_PWRKEY, HIGH);
  delay(10);


  SerialSIM.begin(4800);
  modem.setNetworkMode(13);             // 38-nbiot 13-gsm
  //modem.setPreferredMode(2);          //<-- Uncomment this, if you are using nbiot,.
  modem.getModemName();
  
  modem.restart();
  delay(3000);
  Serial.println("Finding Network...");
  if (!modem.waitForNetwork())
  {
    Serial.println("Network Fail");
    while (true);
  }
  else
  {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect(GSM_APN, "", ""))  //change the APN to NB_APN for nbiot 
    {
      Serial.println(F("GPRS Fail"));  
    }
    else
    {
      Serial.println(getCPSI());
      mqtt.setServer(MQTT_HOST, MQTT_PORT);
      mqtt.setCallback(CallBack);
    }
  }
}

void loop()
{
  
  if (mqtt.connected())
  {
    mqtt.loop();
  }
  else
  {
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
  Serial.println(ButtonState);
 
  if(oldButtonState!=ButtonState) {
    sendVal(ButtonState);
    oldButtonState = ButtonState;
  } else {
     ButtonState = digitalRead(button);
  }
  delay(500); // delay for 0.5 second.
}
