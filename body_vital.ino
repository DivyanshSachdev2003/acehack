#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <MAX30105.h>
#include "heartRate.h"

const char *ssid = "hunya-medtech";
const char *host = "esp32"; // Hostname for the ESP32
WebServer server(80);

// MAX30102 sensor
MAX30105 particleSensor;

// DS18B20 sensor
#define ONE_WIRE_BUS 4 // Pin where the DS18B20 is connected
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Function to handle root url request
void handleRoot() {
  server.send(200, "text/plain", "Welcome to Hunya Medtech");
}

// Function to handle BPM request
void handleBPM() {
  int bpm = getHeartRate();
  server.send(200, "text/plain", String(bpm));
}

// Function to handle SpO2 request
void handleSpO2() {
  int spo2 = getSpO2();
  server.send(200, "text/plain", String(spo2));
}

// Function to handle body temperature request
void handleBodyTemp() {
  float temp = getBodyTemperature();
  server.send(200, "text/plain", String(temp));
}

// Function to read BPM (Heart Rate)
int getHeartRate() {
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  int bpm = particleSensor.getHeartRate();
  return bpm;
}

// Function to read SpO2
int getSpO2() {
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  int spo2 = particleSensor.getSpO2();
  return spo2;
}

// Function to read body temperature
float getBodyTemperature() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  return tempC;
}

void setup() {
  // Start Serial communication
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.softAP(ssid);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Initialize DS18B20 sensor
  sensors.begin();

  // Set up server routes
  server.on("/", handleRoot);
  server.on("/bpm", handleBPM);
  server.on("/spo2", handleSpO2);
  server.on("/bodytemp", handleBodyTemp);

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();
}
