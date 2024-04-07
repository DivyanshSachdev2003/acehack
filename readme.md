Shunya-Medtech ESP32 Health Monitoring System
Introduction
The Shunya-Medtech ESP32 Health Monitoring System is a project aimed at creating a portable health monitoring device using the ESP32 microcontroller. This device can measure various vital signs such as heart rate, blood oxygen saturation (SpO2), body temperature, electrocardiogram (ECG), and body hydration.

Components Used
ESP32 microcontroller
MAX30102 sensor for heart rate and SpO2 measurement
DS18B20 temperature sensor
AD8323 chip for ECG measurement
GSR (Galvanic Skin Response) sensor for body hydration detection
Libraries Required
MAX30105 library
OneWire library
DallasTemperature library
Setup
Connect the MAX30102 sensor, DS18B20 temperature sensor, AD8323 chip, and GSR sensor to the ESP32 microcontroller.
Install the required libraries listed above.
Upload the provided code to the ESP32 microcontroller using the Arduino IDE or any other compatible development environment.
Usage
Power on the ESP32 device.
Connect to the WiFi network named "Shunya-medtech".
Access the health monitoring data via the following endpoints:
/bpm: Get the current heart rate (BPM).
/spo2: Get the current blood oxygen saturation (SpO2).
/bodytemp: Get the current body temperature.
/hydration: Get the current body hydration level.
View the health monitoring data on any device connected to the same WiFi network by accessing the IP address of the ESP32 device.
Contributing
Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please open an issue or create a pull request on GitHub.

License
This project is licensed under the MIT License.
