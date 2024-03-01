  #include <Wire.h>
//#include <LiquidCrystal_I2C.h> // Library for LCD
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include <Servo.h>
#include <SPI.h>

Servo myservo;
int pos=0; 
const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET  -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
#define SENSOR_PIN A0
#define PIN_QUIET 3
#define PIN_MODERATE 4
#define PIN_LOUD 5
#define Buzzer 6
 
void setup ()  
{  
  myservo.attach(7); 
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT);
  pinMode(Buzzer,OUTPUT); 
 
  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);
  pinMode(Buzzer,LOW); 
  
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

   display.clearDisplay();//oled
  display.setTextSize(1);//loed
  display.setTextColor(WHITE);//oled

}  
 
   
void loop ()  
{ 
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,49.5,90);             //calibrate for deciBels
 
  display.setCursor(0, 0);

  display.print("Loudness: ");

  display.print(db);

  display.println("dB");
  display.display();
  if (db <= 52)
  {
   
    display.setCursor(0, 23);
    display.println("Level: Quite");
    display.display();
    digitalWrite(PIN_QUIET, HIGH);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
    pinMode(Buzzer,LOW);
    
    myservo.write(150);
    
  
  }
  else if (db > 52 && db<60)
  {
//    display.clearDisplay();
    display.setCursor(0, 23);

    display.print("Level: Moderate");
    display.display();
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, HIGH);
    digitalWrite(PIN_LOUD, LOW);
    pinMode(Buzzer,LOW);
    
    myservo.write(90);
  
  }
  else if (db>=60)
  
  {
//       display.clearDisplay();
    display.setCursor(0, 23);

     display.print("Level: High");
     display.display();
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, HIGH);
    pinMode(Buzzer,HIGH);
    
    myservo.write(50);
  }
   
   delay(200); 
   display.clearDisplay();
}
