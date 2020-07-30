
#include "dht11.h"
#include <LiquidCrystal.h>  
#include <OneWire.h>
#include <DallasTemperature.h>

#define COOL 1
#define TEMPERATE 2
#define HOT 3

#define LOW_TEMP_LIMIT 65.0
#define HIGH_TEMP_LIMIT 75.0

// digital pins
#define TEMP_PROBE_PIN 6
#define BLUE_LED_PIN 7
#define GREEN_LED_PIN 8
#define RED_LED_PIN 9
#define RELAY_PIN 10

// analog pins
#define DHT_PIN A0 
 
dht11 DHT;


LiquidCrystal lcd(12, 11, 10, 9, 8, 7); 

// initialize the one wire library
OneWire oneWire(TEMP_PROBE_PIN);

// initialize the Dallas temperature library
DallasTemperature sensors(&oneWire);

float probeTemp1 = 0;
float probeTemp2 = 0;

int state = 0;
int activeLEDPin = RED_LED_PIN;


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
 
void setup(){
  lcd.begin(20, 2);  
  lcd.print("DHT and 1 wire");  

  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  Serial.begin(115200);
  
  delay(500);//Delay to let system boot

  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
}

void displayDHT(float rh, float temp)
{
  static char buff[32];

  sprintf(buff, "DHT temp %d.%1d F", (int)temp, (int)(temp*10)%10);

  // set the cursor to column 0, line 'row'  
  lcd.setCursor(0, 0);  
  lcd.print(buff);
  
  sprintf(buff, "Humidity %d.%1d %%", (int)rh, (int)(rh*10)%10);

  lcd.setCursor(0, 1);  
  lcd.print(buff);
}

void displayProbeTemp(int probe, int row, float temp)
{
  static char buff[32];

  sprintf(buff, "Probe %d %d.%1d F", probe, (int)temp, (int)(temp*10)%10);

  // set the cursor to column 0, line 'row'  
  lcd.setCursor(0, row);  
  lcd.print(buff);
}
 
void loop(){

  flashLED(activeLEDPin);

  sensors.requestTemperatures(); 

  delay(1000);

  probeTemp1 = sensors.getTempFByIndex(0);
  probeTemp2 = sensors.getTempFByIndex(1);


    DHT.read(DHT_PIN);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");

    displayDHT(DHT.humidity, DHT.temperature * 9.0/5.0 + 32.0);

    //displayProbeTemp(1, 2, probeTemp1);
   //displayProbeTemp(2, 3, probeTemp2);

  if( probeTemp1 <= LOW_TEMP_LIMIT )
  {
    if( state != COOL )
    {
      state = COOL;
      setDigitalOutputs(HIGH, LOW, LOW, HIGH);
      activeLEDPin = BLUE_LED_PIN;
    }
  }
  else if ( probeTemp1 <= HIGH_TEMP_LIMIT )
  {
    if( state != TEMPERATE )
    {
       state = TEMPERATE;
      setDigitalOutputs(LOW, HIGH, LOW, HIGH);
      activeLEDPin = GREEN_LED_PIN;
    }
  }
  else
  {
    if( state != HOT )
    {
      state = HOT;
      setDigitalOutputs(LOW, LOW, HIGH, LOW);
      activeLEDPin = RED_LED_PIN;
    }
  }
    
  delay(3000);//Wait 5 seconds before accessing sensor again.

}// end loop() 
