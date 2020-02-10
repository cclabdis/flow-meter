#include <Arduino.h>
#include <TM1637Display.h>

// Grove TM1637 display module connection pins (Digital Pins)
#define CLK 2
#define DIO 4

TM1637Display display(CLK, DIO);

volatile int flow_frequency; // Measures flow sensor pulses
float vol = 0.0,l_minute;
unsigned char flowsensor = 3; // YF-S201 water sensor input
unsigned long currentTime;
unsigned long cloopTime;


void flow () // Interrupt function
{
   flow_frequency++;
}

void setup()
{
   display.setBrightness(0x0f);
   display.showNumberDecEx( 0 );

   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH);
   Serial.begin(9600);

   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt

   Serial.println("Water Flow Meter");

   currentTime = millis();
   cloopTime = currentTime;
}

void loop ()
{
   currentTime = millis();
   
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour

      Serial.print("Rate: ");
      Serial.print(l_minute);
      Serial.println(" L/M");

      l_minute = l_minute/60;
      vol = vol + l_minute;

      Serial.print("Vol:");
      Serial.print(vol);
      Serial.println(" L");

      flow_frequency = 0; // Reset Counter

      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");

      display.showNumberDecEx( (vol * 0.264) * 100, 0b01000000 );
    }
    else {
      Serial.println(" flow rate = 0 ");

      Serial.print("Rate: ");
      Serial.print( flow_frequency );
      Serial.println(" L/M");

      Serial.print("Vol:");
      Serial.print(vol);
      Serial.println(" L");
    }
   }
}