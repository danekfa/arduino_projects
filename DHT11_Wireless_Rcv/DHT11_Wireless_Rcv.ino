
#include "dht11.h"
#include <RF24.h>
#include <LiquidCrystal.h>  

// analog pins
#define DHT_PIN A0 
 
dht11 DHT;

LiquidCrystal lcd(3, 2, 10, 9, 8, 7); 

// pin connections for RF 24
// SCK pin 5 to arduino SPI/SCK (D13)
// MOSI pin 6 to arduino SPI/MOSI (D11)
// MISO pin 7 to arduino SPI/MISO (D12)
// arguments are (RF24 pin 3, RF24 pin 4)
RF24 receiver(5, 6);

const byte address[6] = "10101";

String rcvMessage = ""; 

struct Payload
{
  float tempF;
  float humidity;
};

struct Payload payload;
 
void setup(){
  Serial.begin(115200);

  lcd.begin(20, 2);  
  lcd.print("RF24 Rcv");  

  receiver.begin();
  receiver.setChannel(100);
  receiver.setPALevel(RF24_PA_MAX);
  receiver.setDataRate(RF24_250KBPS);
  receiver.openReadingPipe(0, address);
  receiver.startListening();

  delay(1000); //Wait before accessing Sensors

  Serial.println("RF24 Receive\n\n");
}
 
void loop(){
  
  if( receiver.available() )
  { 
      receiver.read( &payload, sizeof(payload) );

      rcvMessage = "T " + String(payload.tempF) + "F, rh " + String(payload.humidity) + "%";
 
      Serial.println(rcvMessage);

      rcvMessage = "Temp " + String(payload.tempF) + "F"; 
      lcd.setCursor(0, 0);  
      lcd.print(rcvMessage);

      rcvMessage = "Humidity " + String(payload.humidity) + "%"; 
      lcd.setCursor(0, 1);  
      lcd.print(rcvMessage);
  }
  
  delay(100);
} 
