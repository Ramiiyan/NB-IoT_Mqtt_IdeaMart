// DEFALUT Configuration for GSM

#include "Settings.h"

void setup()
{
  Serial.begin(115200);
  pinMode(pin, OUTPUT); //led output session 1
  pinMode(button,INPUT_PULLUP);
  
  pinMode(ideaBoard_PWRKEY, OUTPUT);
  digitalWrite(ideaBoard_PWRKEY, HIGH);
  delay(10);


  SerialSIM.begin(4800);
  modem.setNetworkMode(13); // 38-nbiot 13-gsm
  //modem.setPreferredMode(2);  //<-- Uncomment this, if you are using nbiot,.
  modem.getModemName();
  

  modem.restart();
  delay(3000);
  Serial.println("Finding Network...");
  if (!modem.waitForNetwork())
  {
    Serial.println("Network Fail");  // try to restart the Ideamart DevBoard. or Reupload the code. check whether Sim Install properly.
    while (true);
  }
  else
  {
    Serial.println("Network identified.");
    Serial.print("Signal Strength : ");
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect("dialogbb", "", ""))
    {
      Serial.println("GPRS Fail");  //restart the devBoard. 
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
    Serial.println("Connecting MQTT...");
    digitalWrite(ideaBoard_PWRKEY, HIGH);
    delay(180);
    digitalWrite(ideaBoard_PWRKEY, LOW);
    delay(1000);
    ConnectToMQTT();
    delay(100);
  }
  Serial.print("ButtonState :");
  int ButtonState = digitalRead(button);
  Serial.println(ButtonState);
  sendVal(ButtonState);
  delay(1000); // delay for 1 second.
}
