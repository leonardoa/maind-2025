#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LSM6DS3.h>
#include <Wire.h>

LSM6DS3 myIMU(I2C_MODE, 0x6A);
float aX, aY, aZ;

// Variabile per controllare frequenza di campionamento e intervallo di campionamento
unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 500;

const char* ssid = "FabulousNet";    // Sostituisci con il nome della tua rete WiFi
const char* password = "25jan2022";  // Sostituisci con la password della tua rete WiFi

// Supabase credentials
#define supabaseUrl "https://obglvvzwmypfrfltofgr.supabase.co"
#define supabaseKey "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im9iZ2x2dnp3bXlwZnJmbHRvZmdyIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDE2ODg5MDYsImV4cCI6MjA1NzI2NDkwNn0.0kv9_bL5lLibR7Y4Ui6fUIq43MekqHNXAcKyIps2HMA"
#define tableName "scritturagiroacc"

int i = 0;

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

  sensorInizialization();
}

void loop() {
  sendData();  // Update Record of id=1
  delay(1);    // Wait

  // Controlla se è il momento di leggere i dati del sensore
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentTime;

    sensorReading();
  }
}

// Funzione per inizializzare il sensore
void sensorInizialization() {

  if (myIMU.begin() != 0) {
    Serial.println("Accelerometer error");
  } else {
    // Stampa intestazione dati nella seriale
    Serial.println("aX,aY,aZ");
  }
}

// Funzione per leggere i dati dell'accelerometro
void sensorReading() {
  aX = myIMU.readFloatAccelX();
  aY = myIMU.readFloatAccelY();
  aZ = myIMU.readFloatAccelZ();

  Serial.print("AccelX : " + String(aX, 3));
  Serial.print("  ||  AccelY : " + String(aY, 3));
  Serial.print("  ||  AccelZ : " + String(aZ, 3));
  Serial.println();
}

// Funzione per inviare i dati a Supabase
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


  // i++;



  StaticJsonDocument<200> jsonDoc;
  JsonObject valuesObj = jsonDoc.createNestedObject("values");
  valuesObj["x"] = aX;  // 10;
  valuesObj["y"] = aY;  // 2;
  valuesObj["z"] = aZ;  // 3 + i;


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