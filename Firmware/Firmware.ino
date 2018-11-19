#include <Arduino.h>
#include "DHT.h"
#include "SoilMoisture.h"
#include "SolenoidValve.h"
#include <SPI.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Pin Definitions
#define DHT_PIN_DATA  3
#define SOILMOISTURE5V_PIN_SIG  A3
#define SOLENOIDVALVE_PIN_COIL1 5
#define WATERFLOW5V_PIN_SIG 2

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Ethernet
EthernetClient client;


// object initialization
DHT dht(DHT_PIN_DATA);
SoilMoisture soilMoisture5v(SOILMOISTURE5V_PIN_SIG);
SolenoidValve solenoidValve(SOLENOIDVALVE_PIN_COIL1);

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

void setup() {
  //Setup ethernet connection
  Ethernet.init(10);

  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  Ethernet.begin(mac_addr);

  //Check ethernet connection
  if (Ethernet.begin(mac_addr) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }


  //Sensor details
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  //Waterflow sensor
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


void loop() {
  delay(2000);

  //Read sensor data and store in variables
  // DHT22/11 Humidity and Temperature Sensor Reading humidity in % and Temperature in Celcius
  float dhtHumidity = dht.readHumidity();
  float dhtTempC = dht.readTempC();
  //Soil moisture sensor readings
  int soilMoisture5vVal = soilMoisture5v.read();

  if ((millis() - oldTime) > 1000)   // Only process counters once per second
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

  //dhtHumidity, hdtTempC, soilMoisture5vVal, int(flowRate) L/Min
  //totalMilliLitres ml

  if (currentMillis - previousMillis > interval) { //Read once per interval
    previousMillis = currentMillis;
    //All Sensor readings come here
    // DHT22/11 Humidity and Temperature Sensor Reading humidity in % and Temperature in Celcius
    dhtHumidity = (float)dht.readHumidity();
    dhtTempC = (float)dht.readTempC();
    //Soil moisture sensor readings
    soilMoisture5vVal = (int)soilMoisture5v.read();
    currentMillis = 0;
  }

  detachInterrupt(sensorInterrupt);
  flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
  oldTime = millis();
  flowMilliLitres = (flowRate / 60) * 1000;
  totalMilliLitres += flowMilliLitres;
  float totalLitres = totalMilliLitres / 1000;

  unsigned int frac;

  // Reset the pulse counter so we can start incrementing again
  pulseCount = 0;

  // Enable the interrupt again now that we've finished sending output
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  data = "humidity=" + (String (dhtHumidity)) + "temperature=" + (String(dhtTempC)) + "&soilmoisture=" + (String(soilMoisture5vVal)) + "flowrate=" + (String(flowRate)) + "&volume=" + (String(totalMilliLitres));


  //Send sensor values to DB
  if (client.connect("192.168.1.6", 80)) {
    Serial.print("Connectected to MySQL server. Sending data...");

    client.print("POST /project/UI/pages/samples/add.php? HTTP/1.1\n");
    client.print("Host: 192.168.1.6\n");
    client.print("Connection: close\n");
    client.println("Content-Type: aplication/x-www-form-urlencoded\n");
    client.println("Content-Length: ");
    client.println(data.length());
    client.println("\n\n");
    client.print(data);
    Serial.println("Successful");
  }
  else {
    Serial.print("Connection failed");
  }

  //Read from DB to make watering decision
  //Either open / close tap

  //Disconnect client
  if (client.connected()) {
    client.stop();
  }
}

//Solenoid valve control
void open_water() {
  solenoidValve.on();
}

void close_water() {
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