#define BLYNK_TEMPLATE_ID "TMPLllIlyzgB"
#define BLYNK_DEVICE_NAME "Irrigation Project"
#define BLYNK_AUTH_TOKEN "f5fzrNVyBHfUR8jx4t6ecJDiXAjuS0hy"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"
#include "DHT.h"

DHT dht(D4, DHT22);
int temperature, humidity,i=0;
float soilMoisture;
int myHumidityValues[5];
int myTemperatureValues[5];

void fillMyArrays()
{
  myHumidityValues[i]=humidity;
  myTemperatureValues[i]=temperature;
  i++;
  i=i%5;
  checkForDiseas();
}
void checkForDiseas()
{
  int counter = 0;
  for(int i = 0 ; i<5 ; i++)
  {
    if(myHumidityValues[i]>25&&myTemperatureValues[i]<30)
    {
      counter++;
     }
    else
    {
      break;
     }   
  }
   if(counter==5)
    {
       //Blynk.logEvent("disease", " the plant will get infected with (Late blight of tomato and potato) ") ;
       counter=0;
    }
    else if(soilMoisture<30.0)
    {
      //Blynk.logEvent("warning", " Water Your Planet") ;
    }
 
 }
BLYNK_WRITE(V3)
{
  int value = param.asInt();
  digitalWrite(D1,value);
}
void sendSensor()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(); 
  soilMoisture = analogRead(A0);
  soilMoisture=map(soilMoisture,0,1023,100,0); 
  Blynk.virtualWrite(V0, humidity);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, soilMoisture);
}
void setup()
{
  Serial.begin(9600);
  dht.begin();
  BlynkEdgent.begin();
  timer.setInterval(1000, sendSensor); 
  timer.setInterval(12000, fillMyArrays); 
  pinMode(D1,OUTPUT);
}

void loop() 
{
  BlynkEdgent.run();
  timer.run();
}
