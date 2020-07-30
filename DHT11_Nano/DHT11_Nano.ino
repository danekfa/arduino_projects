
#include "dht11.h"
#include <LiquidCrystal.h>  


// analog pins
#define DHT_PIN A0 
 
dht11 DHT;

LiquidCrystal lcd(3, 2, 10, 9, 8, 7); 
 
void setup(){
  lcd.begin(20, 2);  
  lcd.print("DHT 11 Nano");  

  Serial.begin(115200);
  
  delay(500);//Delay to let system boot

  Serial.println("DHT11 Nano\n\n");
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

 
void loop(){

  delay(1000);

  DHT.read(DHT_PIN);
  
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

  displayDHT(DHT.humidity, DHT.temperature * 9.0/5.0 + 32.0);

}// end loop() 
