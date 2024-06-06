
#define BLYNK_TEMPLATE_ID           "TMPL6fNqFvtZU"
#define BLYNK_TEMPLATE_NAME         "GAS LEAK DETECTOR"
#define BLYNK_AUTH_TOKEN            "MhwNT2TIDyEbyCQH1TY1KTk1iXkA4P46"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Kantor";
char pass[] = "samlekom";

const int MQ6 = 33;
int led1 = 21;  
int led2 = 19;
int led3 = 18;
int buzzer = 23;

long RL = 20000; 
long Ro = 200000;

BlynkTimer timer;

void myTimerEvent()
{
 int sensorvalue = analogRead(MQ6); 
 float VRL= sensorvalue*3.30/4095;  
 Serial.print("VRL : ");
 Serial.print(VRL);
 Serial.println(" volt");

 float Rs = (3.30/VRL-1 )*RL; 
 Serial.print("Rs : ");
 Serial.print(Rs);
 Serial.println(" Ohm");

 float D = Rs/Ro;
 Serial.print("Rs/Ro: ");
 Serial.println(D);

double E = D/18.575;
 float ppm = pow(E,-2.375);
 Serial.print("LPG : ");
 Serial.print(ppm);
 Serial.println(" ppm");

 Serial.println();
 delay(1000);

  if(ppm > 18000)
  {
    Serial.println("Peringatan: Terjadi Kebocoran Gas Berpotensi Kebakaran");
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    digitalWrite(buzzer,HIGH);
    Blynk.logEvent("peringatan_kebocoran_gas_berpotensi_kebakaran", String("Peringatan Terjadi Kebocoran Gas Berpotensi Kebakaran: ") + ppm);
  }
  else if(ppm > 2000)
  {
    Serial.println("Peringatan: Terjadi Kebocoran Gas");
    digitalWrite(led1,LOW); 
    digitalWrite(led2,HIGH);
    digitalWrite(led3,LOW);
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,HIGH);
    Blynk.logEvent("peringatan_kebocoran_gas", String("Peringatan Terjadi Kebocoran Gas: ") + ppm);
  } 
  else 
  {
    Serial.println("Tidak Ada Kebocoran Gas");
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(buzzer,LOW);  
    
  }

  Blynk.virtualWrite(V5, ppm);
  Blynk.virtualWrite(V6, VRL);
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void loop()
{
  Blynk.run();
  timer.run();
}
