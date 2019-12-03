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
#include <Wire.h>
#include <DHT.h>
#include <math.h>

/*
 * List of constants for digital pins
 */
#define PIN_D_TEMP_N_HUM 2

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
DHT dht(PIN_D_TEMP_N_HUM, DHTTYPE);

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

    // Sample sound / volume --------------------------------

    soundMeter = 0;
    for(byte i=0; i<32; i++)
    {
        soundMeter += analogRead(PIN_A_VOLUME);
    }
    soundMeter >>= 5;
    soundMeter = 20 * log10(analogRead(soundMeter));

    // Sample temperature and humidity ----------------------

    humidityValue = dht.readHumidity();
    temperatureValue = dht.readTemperature();

    // Debug print ------------------------------------------
 
    Serial.print("Humidity: ");
    Serial.print(humidityValue);
    Serial.println("");
    Serial.print("Temperature: ");
    Serial.print(temperatureValue);
    Serial.println("");
    Serial.print("Dust: ");
    Serial.print(dustValue);
    Serial.println("");
    Serial.print("Volume: ");
    Serial.print(soundMeter);
    Serial.println("");
}
