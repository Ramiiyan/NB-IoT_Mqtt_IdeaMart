#include "Settings.h"

void setup()
{
  Serial.begin(115200);
  pinMode(pin, OUTPUT);
  
  pinMode(ideaBoard_PWRKEY, OUTPUT);
  digitalWrite(ideaBoard_PWRKEY, HIGH);
  delay(10);


  SerialSIM.begin(4800);
  modem.setNetworkMode(38); // 38-nbiot 13-gsm
  modem.setPreferredMode(2);
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
    Serial.println("Network identified.");
    Serial.print("Signal Strength : ");
    Serial.println(modem.getSignalQuality());
    if (!modem.gprsConnect("nbiot", "", ""))
    {
      Serial.println("GPRS Fail");
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

  delay(1000);
  randomSeed(analogRead(0));
  int cnt = random(10, 99);
  Serial.println(cnt);
  sendVal(cnt);
  delay(1000);
}