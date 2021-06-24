
#include <LiquidCrystal.h>  
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(A2,A3);
uint8_t second, minute, hour, wday, day, month, year, ctrl;
                                                                                                                // arduino and lcd
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);   //LCD :  (rs, enable, d4, d5, d6, d7)


const int trigPin = 12; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 11; // Echo Pin of Ultrasonic Sensor
// Trig (Trigger) pin is used to trigger the ultrasonic sound pulses. Echo pin produces a pulse when the reflected signal is received.
String a;
void setup() 
{
Serial.begin(9600); // Starting Serial Terminal
// Starting Serial Terminal  opens serial port, sets data rate to 9600 bps

  //s.println("Begin");
lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width or columns and height or rows) of the display                                                  
pinMode(trigPin, OUTPUT); // Configures the specified pin to behave either as an input or an output. pinMode(pin, mode)
pinMode(echoPin, INPUT);
lcd.setCursor(0,0); //Position the LCD cursor; that is, set the location at which subsequent text written to the LCD will be displayed.
lcd.print("  Seawave    ");
//Serial.print("  Seawave    ");
lcd.setCursor(0,1);
lcd.print("  Measurement  ");
//Serial.print("  Measurement  ");
//Serial.println();
//delay(1000); // Pauses the program for the amount of time (in milliseconds) specified as parameter
lcd.clear(); // Clears the LCD screen and positions the cursor in the upper-left corner.
}

                                                                                                                  // ultrasonic sensor
void loop()
{

  
 

  long duration, inches, cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  

  delay(100);
  Serial.println();

lcd.setCursor(0,0);                                                 
lcd.print("");
delay(10);
lcd.setCursor(0,1);
lcd.print("Height:");
lcd.print(cm/100.0,2);
lcd.print("m");
//s.println(cm);
lcd.setCursor(0,0); 
s.println(cm);

}
/* Sound travels at 343 meters per second, which means it needs 29.155 microseconds per centimeter.
So, we have to divide the duration by 29 and then by 2, because the sound has to travel the distance twice.
 It travels to the object and then back to the sensor.*/
long microsecondsToCentimeters(long microseconds)
{
   return microseconds / 29 / 2;
}
long microsecondsToInches(long microseconds)
{
 return microseconds / 74 / 2;
}
