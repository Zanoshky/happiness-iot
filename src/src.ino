

/*
 * Codename Happiness
 * Language: Arduino
 * 
 * This program reads various sensors
 * <ul>
 * <li>Humidity - DHT22</li>
 * <li>Temperature - DHT22</li>
 * <li>Dust - TODO</li>
 * <li>Volume - SoundSensor LM386</li>
 * <li>Light - BH1750</li>
 * <li>Gas - MQ2</li>
 * <li>Pressure - TODO</li>
 * </ul> 
 * 
 * And sends an HTTP request every seconds with current values.
 * 
 * The circuit:
 * TODO
 * 
 * Created: 2019-11-12
 * by Marko Zanoski <zanoski.marko@gmail.com>
 * assisted by 
 * 
 * This example code is in the public domain on Github.
 */

/*
 * List of libraries
 */
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DHT.h>
#include <math.h>
#include <BH1750.h>
#include <Adafruit_BME280.h>

/*
 * List of constants for digital pins
 */
#define PIN_D_TEMP_N_HUM 2
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11 
#define BME_CS 10

/*
 * List of constants for analog pins
 */
#define PIN_A_GAS 0
#define PIN_A_VOLUME A2

/*
 * List of constants for code
 */
#define TEAM_NAME "Heisenberg"
#define DHTTYPE DHT22

/*
 * Code variables
 */
float humidityValue;
float temperatureValue;
float dustValue;
long soundMeter;
float digitalTemp;
float digitalHumidity;
float digitalPressure;
int pinDust = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
int smokeAnalogSensor = A0;
int sensorThres = 400;

//temperature & humidity
DHT dht(PIN_D_TEMP_N_HUM, DHTTYPE);

//light
BH1750 lightMeter;

//Wifi
//SoftwareSerial esp8266(0, 1);

//digital temperature & humidity
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);


/*
 * Initial code block that executes on startup / booting
 */
void setup()
{
  // Define USB
  Serial.begin(9600);
  //esp8266.begin(115200); 

  dht.begin();
  lightMeter.begin();
  Serial.print("Starting Codename Happiness for ");
  Serial.print(TEAM_NAME);
  Serial.print("...");

  pinMode(pinDust,INPUT);
  starttime = millis(); 

  pinMode(smokeAnalogSensor, INPUT);

  if (!bme.begin()) {  
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
}

/**
 * Compilaiton result
 * Sketch uses 3620 bytes (11%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 252 bytes (12%) of dynamic memory, leaving 1796 bytes for local variables. Maximum is 2048 bytes.
 * 
 * With Sound Reading - LM386
 * Sketch uses 3810 bytes (11%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 256 bytes (12%) of dynamic memory, leaving 1792 bytes for local variables. Maximum is 2048 bytes.
 * 
 * With Light Reading - BH1750
 * Sketch uses 5890 bytes (18%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 514 bytes (25%) of dynamic memory, leaving 1534 bytes for local variables. Maximum is 2048 bytes.
 * 
 * With Temperature and Humidity - DHT22
 * Sketch uses 7424 bytes (23%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 541 bytes (26%) of dynamic memory, leaving 1507 bytes for local variables. Maximum is 2048 bytes.
 * 
 * Code optimization from int to byte
 * Sketch uses 7416 bytes (22%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 537 bytes (26%) of dynamic memory, leaving 1511 bytes for local variables. Maximum is 2048 bytes.
 */

/*
 * Main code logic that loop indefinitely
 */
void loop()
{  
    delay(1000);

    //light BH1750-------------------------------------------
    uint16_t lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    // Sample sound -----------------------------------------
    soundMeter = 0;
    for(byte i=0; i<32; i++)
    {
        soundMeter += analogRead(PIN_A_VOLUME);
    }
    soundMeter >>= 5;
    soundMeter = 20 * log10(analogRead(soundMeter));
    Serial.print("Sound: ");
    Serial.print(soundMeter);
    Serial.println("");

    //DHT-----------------------------------------------------
    humidityValue = dht.readHumidity();
    temperatureValue = dht.readTemperature();
    Serial.print("Humidity: ");
    Serial.print(humidityValue);
    Serial.println("");
    Serial.print("Temperature: ");
    Serial.print(temperatureValue);
    Serial.println("");
   

    //digitalBME280-------------------------------------------
    Serial.print("Digital Values:");
    digitalTemp = bme.readTemperature();
    digitalPressure = bme.readPressure();
    digitalHumidity = bme.readHumidity();
    Serial.print("Digital Temp: ");
    Serial.print(digitalTemp);
    Serial.println("");
    Serial.print("Digital Humidity: ");
    Serial.print(digitalHumidity);
    Serial.println("");
    Serial.print("Digital Pressure ");
    Serial.print(digitalPressure);
    Serial.println("");

    //Smoke detection MQ2-----------------------------------
    int analogSensor = analogRead(smokeAnalogSensor);
    Serial.print("MQ2: ");
    Serial.println(analogSensor);

    //Dust detection-----------------------------------------
    duration = pulseIn(pinDust, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
    if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s 
    {
      ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
      concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
      Serial.print("DustConcentration = ");
      Serial.print(concentration);
      Serial.println(" pcs/0.01cf");
      Serial.println("\n");
      lowpulseoccupancy = 0;
      starttime = millis();
  }
    
}
