// Include Libraries
#include "Arduino.h"
#include "DHT.h"
#include "SoilMoisture.h"
#include "SolenoidValve.h"


// Pin Definitions
#define DHT_PIN_DATA	3
#define ETHERNETMODULE_PIN_CS	10
#define ETHERNETMODULE_PIN_INT	4
#define SOILMOISTURE5V_PIN_SIG	A3
#define SOLENOIDVALVE_PIN_COIL1	5
#define WATERFLOW5V_PIN_SIG	2



// Global variables and defines

// object initialization
DHT dht(DHT_PIN_DATA);
SoilMoisture soilMoisture5v(SOILMOISTURE5V_PIN_SIG);
SolenoidValve solenoidValve(SOLENOIDVALVE_PIN_COIL1);



// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;


//waterflow sensor
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");

    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);

    pulseCount        = 0;
    flowRate          = 0.0;
    flowMilliLitres   = 0;
    totalMilliLitres  = 0;
    oldTime           = 0;

    // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
    // Configured to trigger on a FALLING state change (transition from HIGH
    // state to LOW state)
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
      
    dht.begin();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    //Connect Ethernet Module: Look into having it in the setup instead of Loop section


    //Sensing all environmental conditions
        // DHT22/11 Humidity and Temperature Sensor Reading humidity in % and Temperature in Celcius
        float dhtHumidity = dht.readHumidity();
        float dhtTempC = dht.readTempC();

        //Soil moisture sensor readings
        int soilMoisture5vVal = soilMoisture5v.read();

        if((millis() - oldTime) > 1000)    // Only process counters once per second
        { 
          detachInterrupt(sensorInterrupt);
          flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
          oldTime = millis();
          flowMilliLitres = (flowRate / 60) * 1000;
          totalMilliLitres += flowMilliLitres;
          
          unsigned int frac;
          
          // Reset the pulse counter so we can start incrementing again
          pulseCount = 0;
          
          // Enable the interrupt again now that we've finished sending output
          attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
        }

        //Display readings
        Serial.print(F("Humidity: ")); Serial.print(dhtHumidity); Serial.print(F(" [%]\t"));
        Serial.print(F("Temp: ")); Serial.print(dhtTempC); Serial.println(F(" [C]"));
        Serial.print(F("Val: ")); Serial.println(soilMoisture5vVal);

        Serial.print("Flow rate: ");
        Serial.print(int(flowRate));  // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t");       // Print tab space

        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");        
        Serial.print(totalMilliLitres);
        Serial.println("mL"); 
        Serial.print("\t");       // Print tab space
        Serial.print(totalMilliLitres/1000);
        Serial.print("L");
    
    
    //Turn on solenoid valve depending on sensor readings.
        /*Special Note:
            This should use AI to change the values.

            Test conditions
            
            read soil moisture and compare to optimum value from AI
            25 being the placeholder
        */
        while(soilMoisture5vVal < 25 && dhtHumidity < 75 && dhtTempC < 25){
        /*Special Note:
            Use AI to determine all values 
        */
        
            //Turn on
                solenoidValve.on();

            //Read flow rate to determine how much water is used to irrigate at a certain point
            //Start timer
            //Multiply timer by fow rate to give volume of water used
        }
        
        //Turn off
            solenoidValve.off();

        //delay(10000); //10 sec delay
}


/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
