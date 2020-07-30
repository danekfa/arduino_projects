
#include <LiquidCrystal.h>  
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>

#define TEMP_PROBE_PIN 6
#define BLUE_LED_PIN 7
#define GREEN_LED_PIN 8
#define RED_LED_PIN 9
#define BUTTON_PIN 10
#define RELAY_PIN 13

#define LOW_TEMP_LIMIT 65.0
#define HIGH_TEMP_LIMIT 75.0

#define COOL 1
#define TEMPERATE 2
#define HOT 3


//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=1, Ki=0.01, Kd=0.1;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;


// initialize the one wire library
OneWire oneWire(TEMP_PROBE_PIN);

// initialize the Dallas temperature library
DallasTemperature sensors(&oneWire);

// initialize the LCD library with the pins on the Arduino board  
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  

float fahrenheit = 0;
float fahrenheit1 = 0;

int state = 0;
int activeLEDPin = BLUE_LED_PIN;

void displayTemp(int row, float temp)
{
    // set the cursor to column 0, line 'row'  
  lcd.setCursor(0, row);  
  lcd.print(row + 1);
  lcd.setCursor(1, row);  
  lcd.print(") ");
  lcd.setCursor(3, row);  
  lcd.print(temp, 2);
  lcd.setCursor(9, row);   
  lcd.print("F");  
}

void setDigitalOutputs(int blueLedState, int greenLedState, int redLedState, int relayState)
{
  digitalWrite(BLUE_LED_PIN, blueLedState);
  digitalWrite(GREEN_LED_PIN, greenLedState);
  digitalWrite(RED_LED_PIN, redLedState);
  digitalWrite(RELAY_PIN, relayState);
}

void flashLED(int pin)
{
   digitalWrite(pin, HIGH);
   delay(100);
   digitalWrite(pin, LOW);
}

void setup() {  

 windowStartTime = millis();

 //initialize the variables we're linked to
  Setpoint = 75;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  // Here, 16 and 2 are the columns and rows of the LCD  
  lcd.begin(16, 2);  
  
  lcd.print("PID");  
  
   sensors.begin();

   pinMode(RELAY_PIN, OUTPUT);
   pinMode(BLUE_LED_PIN, OUTPUT);
   pinMode(GREEN_LED_PIN, OUTPUT);
   pinMode(RED_LED_PIN, OUTPUT);
   
  Serial.begin(115200);
}  

void loop() {  

  flashLED(activeLEDPin);

  sensors.requestTemperatures(); 

  delay(1000);

  fahrenheit = sensors.getTempFByIndex(0);
  fahrenheit1 = sensors.getTempFByIndex(1);

  Input = fahrenheit;
  
  myPID.Compute();

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if (millis() - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  
  if (Output > 0.0 ) // < millis() - windowStartTime) {
  {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
  }

  Serial.print(fahrenheit);
  Serial.print(" \t");
  Serial.println(Output * 100);
  

  displayTemp(0, fahrenheit);
  displayTemp(1, fahrenheit1);
 
}  
