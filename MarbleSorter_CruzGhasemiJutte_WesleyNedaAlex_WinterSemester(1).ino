/* Marble sorter arduino code writen by Good Karma team (mar2021) for capstone project course MGMT-3074-2 
 * TCS34725 sensor code for rgb serial print adapted from learnelectronics (mar2018)  

//!!ORIGINAL HEADER FROM THE ONLY SOURCE CODE USED IN THIS PROGRAM:

 * Adafruit TCS34725 RGB Sensor Demo Code
 * 
 * learnelectronics
 * 2 MAR 2018
 * 
 * www.youtube.com/c/learnelectronics
 * www.patreon.com/learnelectronics
 * paypal.me/learnelectronics
 * 
 * adapted from code by Adafruit
 * 
//!!END OF THE SOURCE CODE HEADER
//!!EACH SOURCE CODE SECTION WILL BE INDIVIDUALLY REFERENCED WHEN USED IN THE PROGRAM!! 
//!!ALL AUTORAL CODE SECTIONS WILL NOT BE REFERENCED 

 */
 
#include <Wire.h>  //include the I2C library to communicate with the sensor

#include "Adafruit_TCS34725.h"  //include the sensor library

#include <Servo.h>  //include servo library

#define commonAnode false // set to false if using a common cathode LED, from learnelectronics_2018                    

byte gammatable[256]; //converts color sensor output in eye-recognized gamma color, create an instance of the TCS34725 Sensor, from learnelectronics_2018
                        
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); //from learnelectronics_2018

int led = 8; //define pin 8 for part present led

int photocel = 7; //define pin 7 for photocel 

Servo dropServo; //define code name for drop servo

Servo dragServo; //define code name for drag servo



void setup() 
{ dropServo.attach(13); //define pin 13 for drop servo 
  
  dragServo.attach(12); //define pin 12 for drag servo 
  
  pinMode(led, OUTPUT); // sets the digital pin 8 as output
  
  pinMode(photocel, INPUT); // sets the digital pin 7 as input

//!!start of serial print code section from learnelectronics_2018
  
  Serial.begin(9600); //start serial comms @ 9600
  
  Serial.println("Color View Test"); //print title info             

  if (tcs.begin()) 
  
    { Serial.println("Found sensor"); //if the sensor starts correctly, print the happy message
 
    } else { Serial.println("No TCS34725 found ... check your connections"); //if the sensor starts incorrectly, print the not so happy message
  
while (1); // halt!
                              }
//!!end of serial print code sction from learnelectronics_2018 
                                                                    
//!!start of gamma table conversion from learnelectronics_2018, it helps convert RGB colors to what humans see

for (int i=0; i<256; i++) 
  { float x = i;
  
  x /= 255;
  
  x = pow(x, 2.5);
  
  x *= 255;
      
  if (commonAnode) 
   { gammatable[i] = 255 - x;
   } else { gammatable[i] = x;      
          }
                                             
   }
}

//!!end of gamma table conversion



void loop() 

{ dropServo.write(5); //commands hatch to shutt
  
  delay(1000); //wait 1sec for servo movement

  dragServo.write(5); //move sorting module for home position 
  
  delay(3000); //wait 3sec for servo movement
  
  digitalRead (photocel); //read voltage output from photocell <1.5v or >1.5v
  
  delay(1000); //wait 1sec for input reading
  
  if (photocel = LOW) 
    
    { //if marble is in place photocel resistance increases and voltage decreases below 1.5v = LOW. in LOW case color sorting cycle begins
    
    //the previous if bracket comprises all the rest of the code because if = LOW means a marble to be color classified and appropriately transported 
    
    digitalWrite(led, HIGH); //part present led turns on

    dragServo.write(20); //move sorting module to sorting position 

    //!!start of printing section from learnelectronics (mar2018) 
    
    uint16_t clear, red, green, blue; //declare variables for the colors

    tcs.setInterrupt(false); // turn on LED

    delay(60); // takes 50ms to read 
  
    tcs.getRawData(&red, &green, &blue, &clear); //read the sensor

    tcs.setInterrupt(true); // turn off LED
  
    Serial.print("C:\t"); Serial.print(clear); //print color values
    
    Serial.print("\tR:\t"); Serial.print(red);
            
    Serial.print("\tG:\t"); Serial.print(green);
            
    Serial.print("\tB:\t"); Serial.print(blue);

    // Figure out some basic hex code for visualization
    
    uint32_t sum = clear;
    
    float r, g, b;
    
    r = red; r /= sum;
    
    g = green; g /= sum;
    
    b = blue; b /= sum;
    
    r *= 256; g *= 256; b *= 256;
    
    Serial.print("\t");
    
    Serial.print((int)r, HEX); 
    
    Serial.print((int)g, HEX); 
    
    Serial.print((int)b, HEX);
    
    Serial.println();
 
    Serial.print((int)r ); 
    
    Serial.print(" "); 
    
    Serial.print((int)g);
    
    Serial.print(" ");  
    
    Serial.println((int)b ); 
    
    //!!end of printing section from learnelectronics_2018
 
    if (r>1000 && g>1000 && b>1000) 
      
      { dragServo.write(60);
      
      delay(3000); 
      
      dropServo.write(175);
      
      delay(1000);
      
      exit(0);
      
      }
      
      if (r>b;r>g) 
        
        { dragServo.write(100);
        
        delay(3000); 
        
        dropServo.write(175);
        
        delay(1000);
        
        exit(0);
        
        }
        
        if (g>r;g>b) 
        
          { dragServo.write(140);
          
          delay(3000);
          
          dropServo.write(175);
          
          delay(1000);
          
          exit(0);   
          
          }

          if (b>r;b>g) 
          
            { dragServo.write(180);
            
            delay(3000);
            
            dropServo.write(175);
            
            delay(1000);
            
            exit(0); 
            
            } 

    delay(1000);               // wait for a second
  
  }
  
  else led = LOW;
  
}
