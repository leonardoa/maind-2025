#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "DenWIFI";            // Sostituisci con il nome della tua rete WiFi
const char* password = "Pass4Travel?!";  // Sostituisci con la password della tua rete WiFi

// Supabase credentials
#define supabaseUrl "https://witttxltipffmepbutsg.supabase.co"
#define supabaseKey "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6IndpdHR0eGx0aXBmZm1lcGJ1dHNnIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Mzk1MzcyMjIsImV4cCI6MjA1NTExMzIyMn0.y7sdk3EWA49uTOO2b56rV-O4xKuYaE64JjCiB2HXxng"
#define tableName "realtimedatabase7"

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
}

void loop() {
  sendData();  // Update Record of id=1
  delay(1);    // Wait
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
  valuesObj["x"] = 10;
  valuesObj["y"] = 2;
  valuesObj["z"] = 3 + i;


  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);  // Serialize the JSON document to a string

  int httpResponseCode = http.PATCH(jsonString);  // PATCH the JSON data
  String response = http.getString();             // Get the response from the server

 
  if (httpResponseCode > 0) {
    //Serial.print("HTTP Response Code: ");
    //Serial.println(httpResponseCode);
    //Serial.println("Response: " + response);
  } else {
    Serial.print("Error in sending PATCH request: ");
    Serial.println(http.errorToString(httpResponseCode));
  }

  http.end();
}