
#include <Wire.h>
#include <Adafruit_MLX90614.h>
//#include "SparkFunHTU21D.h"
#include "Adafruit_HTU21DF.h"
#include <movingAvg.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
//Create an instance of the object
//HTU21D myHumidity;
Adafruit_HTU21DF myHumidity = Adafruit_HTU21DF();

const long oneSecond = 1000;  // a second is a thousand milliseconds
const long oneMinute = oneSecond * 60;
const long oneHour   = oneMinute * 60;
const long oneDay    = oneHour * 24;

const int analogOutPin = 12; // Analog output pin that the motor is attached to
const int LED = 6; //Led Pin
const int Water_Sensor=5;
const int Heat_Ex=4;
const int Fan=3;
const int Fan_Sensor = 2;
int Fan_Speed;
int outputValue = 0;        // value output to the PWM (analog out)
int ObjectTempOffset =5; //Window temp offset + differential below top and bottom of glass
boolean motor_running;

unsigned long previousMillis,currentMillis;
int interval=1000;

movingAvg avgATemp(10);                  // define the moving average object
movingAvg avgHumid(10);                  // define the moving average object
movingAvg avgOTemp(10);                  // define the moving average object
void setup() {
  
  motor_running =false;
  Serial.begin(57600);
  Serial.println("Adafruit MLX90614 test");  
  myHumidity.begin();
  mlx.begin();  
  avgATemp.begin();                            // initialize the moving average
  avgOTemp.begin();                            // initialize the moving average
  avgHumid.begin();                            // initialize the moving average
  pinMode(Fan,OUTPUT);
  
  pinMode(Heat_Ex,OUTPUT);
  pinMode(Water_Sensor,INPUT);
  pinMode(Fan_Sensor,INPUT);
  pinMode(LED,OUTPUT);
  
}


void loop() {

  // put your main code here, to run repeatedly:
 float AmbientTemp = mlx.readAmbientTempC(); 
 //Serial.print("Ambient = "); Serial.print(AmbientTemp);
 float readObjectTemp = mlx.readObjectTempC(); 
 //readObjectTemp = avgOTemp.reading(readObjectTemp);             // calculate the moving average

  float humd = myHumidity.readHumidity();
  //humd = avgHumid.reading(humd);             // calculate the moving average
  float temp = myHumidity.readTemperature();
  //temp = avgATemp.reading(temp);             // calculate the moving average
  float dewp= temp-((100-humd)/5);


unsigned long currentMillis = millis();
if ((unsigned long)(currentMillis - previousMillis) >= interval) {
  Serial.print(" MLX Air Temp");
  Serial.print(AmbientTemp,1);
  Serial.print(" ");
  Serial.print("*C Object = "); Serial.print(readObjectTemp);
  Serial.print(" HTU21D Air Temp: ");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" ");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("% ");
  Serial.print("Dew Point: ");
  Serial.print(dewp,1);
  Serial.print("C");
  Serial.println();
  previousMillis = currentMillis;

  
  if ((readObjectTemp-ObjectTempOffset) <dewp)
  {
    motor_running = true;
    Serial.print("All On");
  }
 
  
 if (readObjectTemp-(ObjectTempOffset+1)>dewp)
      {
        motor_running = false;
        Serial.print("All Off");
      }

Fan_Speed=digitalRead(Fan_Sensor);
Serial.print("Fan Sensor="+Fan_Speed);
if( digitalRead(Water_Sensor)==false)   {
    digitalWrite(LED,LOW);
 if(motor_running == false){
    digitalWrite(Fan,0);
    digitalWrite(Heat_Ex,0);
    Serial.print("Fan Heat Ex. off");
   
    Serial.print("Time:");
    
  
   
    
   }
 if(motor_running == true){
  
     digitalWrite(Fan,1);
    digitalWrite(Heat_Ex,1);
   
   }
}
else
{ Serial.print("Water_Sensor true all Off");
digitalWrite(Fan,0);
digitalWrite(Heat_Ex,0);
digitalWrite(LED,HIGH);
}
   Serial.println();
}
  
}
