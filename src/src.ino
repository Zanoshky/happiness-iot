/*
 * Codename Happiness
 * Language: Arduino
 * 
 * This program reads various sensors
 * <ul>
 * <li>Humidity - DHT22</li>
 * <li>Temperature - DHT22</li>
 * <li>Dust</li>
 * <li>Volume - SoundSensor LM386</li>
 * <li>Light - BH1750</li>
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
#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>



/*
 * List of constants
 */
 #define PIN_D_TEMP_N_HUM 2
//#define PIN_D_DUST 3
//#define PIN_A_VOLUME 0
//#define PIN_A_LIGHT 5


//BM280
#define BME_SCK 13 // Serial Clock -------> SCL
#define BME_MISO 12 // Serial Data Out ---> SDA/SDI
#define BME_MOSI 11 // Serial Data In  ---> SDO
#define BME_CS 10 // Chip Selecta      ---> CSB

#define TEAM_NAME "Heisenberg"

#define DHTPIN 7
#define DHTTYPE DHT22

const int pinAdc = A2;


/*
 * Code variables
 */
float humidityValue;
float temperatureValue;
DHT dht(PIN_D_TEMP_N_HUM, DHTTYPE);

int smokeAnalogSensor = A0;
int sensorThres = 400;

//Adafruit_BME280 bme(BME_CS,BME_MOSI,BME_MISO,BME_SCK);



/*
 * Initial code block that executes on startup / booting
 */
void setup()
{
  // Define USB
  Serial.begin(9600);



  dht.begin();
 

  Serial.print("Starting Codename Happiness for ");
  Serial.print(TEAM_NAME);
  Serial.print("...");


  

//  Serial.println("Trying to wire...");
//  Wire.begin();
//  lightMeter.begin();
//  Serial.println("BH1750 Test");
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

    //Smoke detection 
    int analogSensor = analogRead(smokeAnalogSensor);
    Serial.print("MQ2: ");
    Serial.println(analogSensor);


    // Sample temperature and humidity ----------------------

    humidityValue = dht.readHumidity();
    temperatureValue = dht.readTemperature();

    

    // Debug print ------------------------------------------
    //DHT22 values
    Serial.print("Humidity: ");
    Serial.print(humidityValue);
    Serial.println("");
    Serial.print("Temperature: ");
    Serial.print(temperatureValue);
    Serial.println("");

    long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinAdc);
    }

    sum >>= 5;

    Serial.println(sum);
    delay(10);

    //BME280
//   
//    Serial.print("DigitalTemperature = ");
//    Serial.print(bme.readTemperature());
//    Serial.println("*C");
//
//    Serial.print("DigitalPressure = ");
//    Serial.print(bme.readPressure() / 100.0F);
//    Serial.println("hPa");
//  
//    Serial.print("DigitalHumidity = ");
//    Serial.print(bme.readHumidity());
//    Serial.println("%");

    //BH1750
//    float lux = lightMeter.readLightLevel(true);
//    Serial.print("Light: ");
//    Serial.print(lux);
//    Serial.println(" lx");
}
