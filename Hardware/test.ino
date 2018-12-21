#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define ECHOPIN 13
#define TRIGPIN 12

TinyGPSPlus gps;


SoftwareSerial s2(9,10); //for gsm 

void setup()
{
  Serial.begin(9600); //for gps
  
  s2.begin(9600); //for gsm
  pinMode(ECHOPIN,INPUT);
  pinMode(TRIGPIN, OUTPUT);
  delay(10000); //delay 10seconds
  initGPRS();   
}
void loop()
{
 
  double la, ln,temp;
  while (Serial.available() > 0)  //while gps gets signal
  {
    gps.encode(Serial.read());
  
      while (gps.location.isUpdated())
      {
         temp=analogRead(A0);
         temp=temp*0.48828125;
         digitalWrite(TRIGPIN,LOW);
         delay(25000);
         digitalWrite(TRIGPIN, HIGH);
         delayMicroseconds(10000);
         digitalWrite(TRIGPIN,LOW);
   
         float distance =pulseIn(ECHOPIN, HIGH);
         //delay(10000); 
         distance=distance/58;
         Serial.println(distance);
         Serial.print ("temp= ");
         Serial.println(temp);
         Serial.print("Latitude= "); 
         la=gps.location.lat();
         Serial.println(la, 6);
         Serial.print(" Longitude= ");
         ln=gps.location.lng();
         Serial.println(ln, 6);  
         s2.begin(9600);
         serverdata(la,ln,distance,temp);
         //delay(5000);
      
      }
   }
    
}

void initGPRS()
{
  s2.println("AT");
  delay(200);
  s2.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
  delay(500);
  s2.println("AT+SAPBR=3,1,\"APN\",\"blweb\"\r\n");
  delay(500);
  s2.println("AT+SAPBR =1,1");
  delay(3500);
  s2.println("AT+HTTPINIT");
  delay(500);
  s2.println("AT+HTTPPARA=\"CID\",1");
  delay(500);

}
float serverdata(double longitude, double latitude, double distance, double temp)
{
  //s2.listen();
  s2.print("AT+HTTPPARA=\"URL\",\"http://waste.dgted.com/process.php?binid=1104&longitude=");
  s2.print(latitude, 6);
  s2.print("&lattitude=");
  s2.print(longitude, 6);
  s2.print("&value=");
  s2.print(distance);
  s2.print("&temperature=");
  s2.print(temp);
  s2.println("\"");
  delay(200);
  s2.println(" AT+HTTPACTION=0");
  //delay(100);
  
}
