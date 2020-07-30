 

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int CLK_PIN = 11;  // Pin 11 to clk on encoder
int DT_PIN = 12;  // Pin 12 to DT on encoder

int enc_count = 90; 
int prev_clk;  

boolean clockwise;

void setup() 
{ 
 Serial.begin (115200);

 myservo.attach(13);  // attaches the servo on pin 13 to the servo object
 
 pinMode (CLK_PIN, INPUT);
 pinMode (DT_PIN, INPUT);
 
 prev_clk = digitalRead(CLK_PIN);   
} 

void loop() 
{ 
   int current_clk = digitalRead(CLK_PIN);
   
   // clock pin has changed
   if (current_clk != prev_clk)
   { 
     // we use the DT pin to find out which way we turning.
     int dt = digitalRead(DT_PIN);
   
     clockwise = (dt != current_clk);
     
     if ( clockwise ) 
     {  
       enc_count++;
     } 
     else 
     { 
       enc_count--;
     }

     if( (enc_count % 2) == 0  )
     {
       Serial.print("CLK: ");
       Serial.println(current_clk);
       
       Serial.print("DT: ");
       Serial.println(dt);

       Serial.println(enc_count/2);

       myservo.write(enc_count);  
     }

		// divide by two because the detent on the encoder is for one cycle of rising then falling edge
 } 

 prev_clk = current_clk;
} 
