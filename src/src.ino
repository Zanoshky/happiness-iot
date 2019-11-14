/*
 * Codename Happiness
 * Language: Arduino
 * 
 * This program reads various sensors
 * <ul>
 * <li>Humidity</li>
 * <li>Temperature</li>
 * <li>Dust</li>
 * <li>Volume</li>
 * <li>Light</li>
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
 * TODO
 */

/*
 * List of constants
 */
#define PIN_HUMIDITY 1
#define PIN_TEMPERATURE 2
#define PIN_DUST 3
#define PIN_VOLUME 4
#define PIN_LIGHT 5


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
float volumeValue;
float lightValue;

/*
 * Initial code block that executes on startup / booting
 */
void setup()
{
  Serial.begin(9600);
}

/**
 * Compilaiton result
 * Sketch uses 3620 bytes (11%) of program storage space. Maximum is 32256 bytes.
 * Global variables use 252 bytes (12%) of dynamic memory, leaving 1796 bytes for local variables. Maximum is 2048 bytes.
 */

/*
 * Main code logic that loop indefinitely
 */
void loop()
{
    delay(1000);
    
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
    Serial.print(volumeValue);
    Serial.println("");
    Serial.print("Light: ");
    Serial.print(lightValue);
    Serial.println("");
}
