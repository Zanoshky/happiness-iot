#include <ArduinoJson.h>
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
const byte rxPin = 4; // Wire this to Tx Pin of ESP8266
const byte txPin = 5; // Wire this to Rx Pin of ESP8266

String AP = "";   // AP NAME
String PASS = ""; // AP PASSWORD
String API = "";  // Write API KEY
String HOST = "https://my-office-happiness.com";
String PORT = "9443";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;

int idHomebase = 1;
// ESP8266
SoftwareSerial ESP8266(rxPin, txPin);
// temperature & humidity

DHT dht(PIN_D_TEMP_N_HUM, DHTTYPE);

// light
BH1750 lightMeter;

// digital temperature & humidity
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

void setup()
{
  // Define USB
  Serial.begin(9600);
  ESP8266.begin(9600);

  dht.begin();
  lightMeter.begin();
  Serial.print("Starting Codename Happiness for ");
  Serial.print(TEAM_NAME);
  Serial.print("...");
  Serial.println("");

  pinMode(pinDust, INPUT);
  starttime = millis();

  pinMode(smokeAnalogSensor, INPUT);

  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void sendCommand(String command, int maxTime, char readReplay[])
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    ESP8266.println(command);     // at+cipsend
    if (ESP8266.find(readReplay)) // ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("Works");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}

float readHumidityDH11()
{
  humidityValue = dht.readHumidity();
  return float(humidityValue);
}

float readTemperatureDH11()
{
  temperatureValue = dht.readTemperature();
  return float(temperatureValue);
}

float readDigitalTemperature()
{
  digitalTemp = bme.readTemperature();
  return float(digitalTemp);
}

float readDigitalHumidity()
{
  digitalHumidity = bme.readHumidity();
  return float(digitalHumidity);
}

float readDigitalPressure()
{
  digitalPressure = bme.readPressure();
  return float(digitalPressure);
}

float gasDetection()
{
  int analogSensor = analogRead(smokeAnalogSensor);
  return float(analogSensor);
}

float soundLevelDetection()
{
  soundMeter = 0;
  for (byte i = 0; i < 32; i++)
  {
    soundMeter += analogRead(PIN_A_VOLUME);
  }
  soundMeter >>= 5;
  soundMeter = 20 * log10(analogRead(soundMeter));
  return float(soundMeter);
}

float lightIntensity()
{
  uint16_t lux = lightMeter.readLightLevel();
  return float(lux);
}

float dustLevelDetection()
{
  // Dust detection-----------------------------------------
  duration = pulseIn(pinDust, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  if ((millis() - starttime) >= sampletime_ms)
  {
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0);
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;
    return float(concentration);
    lowpulseoccupancy = 0;
    starttime = millis();
  }
}

void loop()
{
  //   String getData = "GET /update?api_key="+ API +"&field1="+readTemperatureDH11()+"&field2="+readHumidityDH11()+"&field3="+readDigitalTemperature()+"&field4="+readDigitalHumidity()+"&field5="+readDigitalPressure()+"&field6="+lightIntensity()+"&field7="+soundLevelDetection()+"&field8="+dustLevelDetection();
  //   sendCommand("AT+CIPMUX=1",5,"OK");
  //   sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  //   sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  //   ESP8266.println(getData);delay(1500);countTrueCommand++;
  //   sendCommand("AT+CIPCLOSE=0",5,"OK");

  Serial.println("testststtsts ");

  StaticJsonDocument<100> doc;
  doc["timestamp"] = "2020-01-16T16:14:47.995Z";
  doc["temperature"] = readTemperatureDH11();
  doc["humidity"] = readHumidityDH11();
  doc["dust"] = dustLevelDetection();
  doc["gas"] = gasDetection();
  doc["volume"] = soundLevelDetection();
  doc["light"] = lightIntensity();
  doc["homebaseId"] = "1";

  Serial.println("testststtsts ");

  char buffer[100];
  serializeJson(doc, buffer);

  delay(1000);
  Serial.println(buffer);

  String getData = "POST /measurements?access_token="
                   " \r\nHost:" +
                   HOST + ":" + PORT + "\r\nContent-Type: application/json\r\n ";
  Serial.println(getData);

  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  ESP8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");

  //  String uri = "/homebases/1/measurements";
  //  String  data = "temperature=" + readTemperatureDH11() +
  //    "&humidity=" + readHumidityDH11() +
  //    "&dust=" + dustLevelDetection() +
  //    "&gas=" + readDigitalPressure() +
  //    "&volume=" + soundLevelDetection() +
  //    "&light=" + lightIntensity() +
  //    "&homebaseId=1" +
  //    "&timestamp=2020-01-16T16:37:41.138Z";
  //  String postRequest =
  //    "POST " + uri + " HTTP/1.0\r\n" +
  //    "Host: " + HOST + "\r\n" +
  //    "Accept: *" + "/" + "*\r\n" +
  //    "Content-Length: " + data.length() + "\r\n" +
  //    "Content-Type: application/x-www-form-urlencoded\r\n" +
  //    "\r\n" + data;
  //   sendCommand("AT+CIPMUX=1",5,"OK");
  //   sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  //   sendCommand("AT+CIPSEND=" +String(postRequest.length()+4),4,">");
  //   //ESP8266.println(postRequest);delay(1500);countTrueCommand++;
  //

  if (ESP8266.find(">"))
  {
    Serial.println("Sending..");
    ESP8266.println(getData);
    delay(1500);
    countTrueCommand++;
    if (ESP8266.find("SEND OK"))
    {
      Serial.println("Packet sent");
    }
  }
  sendCommand("AT+CIPCLOSE=0", 5, "OK");

  delay(1000);
}
