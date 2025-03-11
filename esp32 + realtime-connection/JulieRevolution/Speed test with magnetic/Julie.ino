#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "FabulousNet";    // Sostituisci con il nome della tua rete WiFi
const char* password = "25jan2022";  // Sostituisci con la password della tua rete WiFi

// Supabase credentials i put mine !
#define supabaseUrl "https://bynxrcltjanjczfelhpp.supabase.co"
#define supabaseKey "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImJ5bnhyY2x0amFuamN6ZmVsaHBwIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDEyNjc0NjAsImV4cCI6MjA1Njg0MzQ2MH0.VqTvM07Ir1v71X2Pxi37KyNXk7wIB4IKyqZUVbD1PDA"
#define tableName "realtimedatabase3"

//set up for the hall sensor / speed
int hallSensorPin = 3;
int hallSensorValue = HIGH;      // true = 1 = HIGH = no magnet
int hallSensorValue_old = HIGH;  // Store previous state
unsigned long lastTime = 0;      // Stores previous revolution time
int speedLevel = 0;

int i = 0;

//millis trick
unsigned long lastSendDataTime = 0;
const unsigned long sendDataInterval = 1000; // Send data every 1000 ms (1 second)

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //set up hall sensor
  pinMode(hallSensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  processHallSensor();
  if (millis() - lastSendDataTime >= sendDataInterval) {
    sendData();
    lastSendDataTime = millis();
  }
}

void sendData() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Attempting to reconnect...");
    while (!WiFi.reconnect()) {
      Serial.println("Reconnecting to WiFi...");
      delay(500);
    }
    Serial.println("WiFi reconnected.");
  }


  HTTPClient http;
  String endpoint = String(supabaseUrl) + "/rest/v1/" + tableName + "?id=eq.1";  // Update row with id=1
  http.begin(endpoint);

  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabaseKey);
  http.addHeader("Authorization", "Bearer " + String(supabaseKey));
  http.addHeader("Prefer", "return=representation");


  i++;

  StaticJsonDocument<200> jsonDoc;
  JsonObject valuesObj = jsonDoc.createNestedObject("values");
  valuesObj["speed"] = speedLevel;
  //valuesObj["y"] = 2;
  //valuesObj["z"] = 3 + i;

  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);  // Serialize the JSON document to a string

  int httpResponseCode = http.PATCH(jsonString);  // PATCH the JSON data
  String response = http.getString();             // Get the response from the server

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error in sending PATCH request: ");
    Serial.println(http.errorToString(httpResponseCode));
  }

  http.end();
}

void processHallSensor() {

  hallSensorValue = digitalRead(hallSensorPin);  // Read Hall sensor
                                                 // Detect falling edge (HIGH -> LOW transition)
  if (hallSensorValue == LOW && hallSensorValue_old == HIGH) {

    unsigned long currentTime = millis();                      // Get current time
    unsigned long timePerRevolution = currentTime - lastTime;  // Time difference

    if (lastTime > 0) {

      float rps = 1000.0 / timePerRevolution;
      Serial.print("Speed: ");
      Serial.print(rps);
      Serial.println(" RPS");

      if (rps > 20) {
        speedLevel = 3;
      } else if (rps > 17 && rps <= 19) {
        speedLevel = 2;
      } else if (rps > 13 && rps <= 16) {
        speedLevel = 1;
      } else {
        speedLevel = 0;
      }

      Serial.print("Speed Level: ");
      Serial.println(speedLevel);
    }

    lastTime = currentTime;  // Update lastTime
  }

  hallSensorValue_old = hallSensorValue;  // Store current sensor state
}