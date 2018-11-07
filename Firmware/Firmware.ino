// Include Libraries
#include "Arduino.h"
#include "DHT.h"
#include "SoilMoisture.h"
#include "SolenoidValve.h"
#include "Ethernet.h"
#include "SPI.h"


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
long time0;

//Ethernet
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01}; //Reserve MAC Address
EthernetClient client;

//waterflow sensor
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;
float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

//Variables for timer
long previousMillis = 0;
unsigned long currentMillis = 0;
long interval = 250000; //Reading Interval

//Data Variables
float dhtHumidity = dht.readHumidity();
float dhtTempC = dht.readTempC();
int soilMoisture5vVal = soilMoisture5v.read();

String data;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages

    Serial.begin(9600);
    
    //Check ethernet connection
    if (Ethernet.begin(mac) == 0){
      Serial.println("Failed to configure Ethernet using DHCP");
    }
    
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
    delay(10000); //Give sensor time to start

   dhtHumidity = (float)dht.readHumidity();
   dhtTempC = (float)dht.readTempC();
   soilMoisture5vVal = (int)soilMoisture5v.read();

    data = "";
}


// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    //Connect Ethernet Module: Look into having it in the setup instead of Loop section
    while (Serial.available()){
        delay(30000); //30 sec delay
        char c = Serial.read();
        readString += c;
    }

    //if read string = "get" show all values
    if (readString == "get"){

    }


    currentMillis = millis();
    if(currentMillis - previousMillis >= interval){ //Read once per interval
      previousMillis += interval;

      //All Sensor readings come here
      // DHT22/11 Humidity and Temperature Sensor Reading humidity in % and Temperature in Celcius
      dhtHumidity = (float)dht.readHumidity();
      dhtTempC = (float)dht.readTempC();

      //Soil moisture sensor readings
      soilMoisture5vVal = (int)soilMoisture5v.read();

    }
   
      
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
        

    //Display readings
    Serial.print(F("Humidity: ")); Serial.print(dhtHumidity); Serial.print(F(" [%]\t"));
    Serial.print(F("Temp: ")); Serial.print(dhtTempC); Serial.println(F(" [C]\t"));
    Serial.print(F("Soil Moisture: ")); Serial.println(soilMoisture5vVal); Serial.print("\t");

    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min \t");
    
    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres/1000);
    Serial.print("L \n");
    
    
}

//Solenoid valve control
void open_water(){
    solenoidValve.on();
}

void close_water(){
    solenoidValve.off();
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
