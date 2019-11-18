/*
 * Codename Happiness
 * Language: Arduino
 * 
 * This program reads various sensors
 * <ul>
 * <li>Humidity</li>
 * <li>Temperature</li>
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

/*
 * List of constants
 */
#define PIN_D_HUMIDITY 1
#define PIN_D_TEMPERATURE 2
#define PIN_D_DUST 3
#define PIN_A_VOLUME 0
#define PIN_A_LIGHT 5
#define TEAM_NAME "Heisenberg"
/*
 * Code initialization
 * TODO
 */


/*
 * Code variables
 */
float humidityValue;
float temperatureValue;
float dustValue;
long soundMeter;
BH1750 lightMeter;

/*
 * Initial code block that executes on startup / booting
 */
void setup()
{
  // Define USB
  Serial.begin(9600);

  lightMeter.begin();

  Serial.print("Starting Codename Happiness for ");
  Serial.print(TEAM_NAME);
  Serial.print("...");
}

/**
 * Compilaiton result
 * Sketch uses 3620 bytes (11%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 252 bytes (12%) of dynamic memory, leaving 1796 bytes for local variables. Maximum is 2048 bytes.
 * 
 * With Sound Reading
 * Sketch uses 3810 bytes (11%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 256 bytes (12%) of dynamic memory, leaving 1792 bytes for local variables. Maximum is 2048 bytes.
 * 
 * With Light Reading
 * Sketch uses 5890 bytes (18%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 514 bytes (25%) of dynamic memory, leaving 1534 bytes for local variables. Maximum is 2048 bytes.
 */

/*
 * Main code logic that loop indefinitely
 */
void loop()
{  
    delay(1000);

    // Sample sound / volume
    soundMeter = 0;
    for(int i=0; i<32; i++)
    {
        soundMeter += analogRead(PIN_A_VOLUME);
    }

    soundMeter >>= 5;

    // Sample light
    uint16_t lightMeterReading = lightMeter.readLightLevel();
    
    Serial.print("Humidity: ");
    Serial.print(humidityValue);
    Serial.println("");
    Serial.print("Temperature: ");
    Serial.print(temperatureValue);
    Serial.println("");
    Serial.print("Dust: ");
    Serial.print(dustValue);
    Serial.println("");
    Serial.print("Volumeity: ");
    Serial.print(soundMeter);
    Serial.println("");
    Serial.print("Light: ");
    Serial.print(lightMeterReading);
    Serial.println("");
}
