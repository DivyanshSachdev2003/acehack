#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const int gsrPin = 32;
int gsrValue; // Global variable to store GSR value
const int numReadings = 10;
int gsrReadings[numReadings];
int gsrIndex = 0;
int gsrTotal = 0;
int gsrAverage = 0;
int a = 500;
int b = 2300;
int calibrationPin = 33;

const char* ssid = "SHUNYA - HYDRATION";
const char* password = "";

AsyncWebServer server(80);

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SHUNYA HYDRATION TESTER</title>
    <style>
            body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f0f0f0;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .container {
            text-align: center;
            padding: 20px;
            border-radius: 20px;
            background-color: #fff;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
            width: 80%;
            max-width: 600px;
        }

        h1 {
            color: #333;
            font-size: 2.5rem;
            margin-bottom: 20px;
        }

        .status {
    font-size: 2rem;
    padding: 20px;
    border-radius: 10px;
    display: inline-block;
    background-color: #ffff00; /* Red */
    font-weight: 600; /* Semi-bold */
    margin: 0 auto; /* Centers the element horizontally */
}

        /* Color variations */
        .hydrated {
            color: #fff;
            background-color: #4CAF50; /* Green */
        }

        .dehydrated {
            color: #fff;
            background-color: #F44336; /* Red */
        }

        .wear {
            color: #fff;
            background-color: #2196F3; /* Blue */
        }
        </style>
</head>
<body>
    <div class="container">
        <h1>SHUNYA Hydration Detector</h1>
        <div id="status" class="status">Calibrating Please Wait...</div>
    </div>
<script>
        function changeBackgroundColor(text) {
            var body = document.body;
            var statusDiv = document.getElementById('status');
            switch (text) {
                case 'Hydrated':
                    body.style.backgroundColor = '#4CAF50'; // Green
                    statusDiv.style.backgroundColor = '#4CAF50'; // Green
                    break;
                case 'Dehydrated':
                    body.style.backgroundColor = '#F44336'; // Red
                    statusDiv.style.backgroundColor = '#F44336'; // Red
                    break;
                case 'Please Wear Your Device':
                    body.style.backgroundColor = '#2196F3'; // Blue
                    statusDiv.style.backgroundColor = '#2196F3'; // Blue
                    break;
                case 'Calibrating Please Wait...':
                    body.style.backgroundColor = '#ffff00'; // Yellow
                    statusDiv.style.backgroundColor = '#ffff00'; // Yellow
                    break;
                default:
                    body.style.backgroundColor = '#fff'; // Default to white
                    statusDiv.style.backgroundColor = '#fff'; // Default to white
            }
        }

        // Call the function to change background color after 20 seconds
        setTimeout(function() {
            setInterval(function() {
                fetch('/status')
                    .then(response => response.text())
                    .then(data => {
                        document.getElementById('status').innerHTML = data;
                        changeBackgroundColor(data); // Call the function to change background color
                    });
            }, 1000);
        }, 20000);
    </script>

</body>
</html>
)=====";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Page Not found");
}

String getHydrationStatus() {
    if (gsrValue > a && gsrValue < b) {
        return "Hydrated";
    }
     else if (gsrValue > 2300) {
        return "Please Wear Your Device";
    } else {
        return "Dehydrated";
    }
}

void setup() {
    Serial.begin(9600);
    WiFi.softAP(ssid, password);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", webpage);
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        String status = getHydrationStatus();
        request->send(200, "text/plain", status);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
    gsrValue = readGSR();
     Serial.print("GSR Value: ");
  Serial.println(gsrValue);
  Serial.print("Hydration Status: ");
  if (gsrValue > a and gsrValue < b) {
    Serial.println("Hydrated");
  } 
  else if(gsrValue > 2350){
    Serial.println("Wear Your Device");
  }
  else {
    Serial.println("Dehydrated");
  }
  delay(2500);
}

int readGSR() {
    gsrTotal -= gsrReadings[gsrIndex];
    gsrReadings[gsrIndex] = analogRead(gsrPin);
    gsrTotal += gsrReadings[gsrIndex];
    gsrIndex = (gsrIndex + 1) % numReadings;
    gsrAverage = gsrTotal / numReadings;
    return gsrAverage;
}
