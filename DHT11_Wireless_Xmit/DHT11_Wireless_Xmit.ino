
#include "dht11.h"
#include <RF24.h>

// analog pins
#define DHT_PIN A0 
 
dht11 DHT;

// pin connections for RF 24
// SCK pin 5 to arduino SPI/SCK (D13)
// MOSI pin 6 to arduino SPI/MOSI (D11)
// MISO pin 7 to arduino SPI/MISO (D12)
// arguments are (RF24 pin 3, RF24 pin 4)
RF24 transmitter(7, 8);

const byte address[6] = "10101";

struct Payload
{
  float tempF;
  float humidity;
};

struct Payload payload;

String txString = "";
char txChar[32] = "";
 
void setup(){
  Serial.begin(115200);

  transmitter.begin();
  transmitter.setChannel(100);
  transmitter.setPALevel(RF24_PA_MAX);
  transmitter.setDataRate(RF24_250KBPS);
  transmitter.openWritingPipe(address);
  transmitter.stopListening();

  delay(1000); //Wait before accessing Sensors

  Serial.println("RF24 Transmit\n\n");
}

 
void loop(){

  DHT.read(DHT_PIN);
  
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

  payload.tempF = DHT.temperature * 9.0/5.0 + 32.0;
  payload.humidity = DHT.humidity;

  transmitter.write(&payload, sizeof(payload));

  delay(1000);
} 
