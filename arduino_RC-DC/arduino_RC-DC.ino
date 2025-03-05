#include <WiFi.h>

const char* ssid = "farasyn 1";        // Replace with your WiFi SSID
const char* password = "spongebob"; // Replace with your WiFi password

WiFiServer server(1234);  // ESP32 server listening on port 1234

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Start the server
  server.begin();
  Serial.println("ESP32 is now connected to WiFi");
}

void loop() {
  WiFiClient client = server.available();  // Check if a client is connected

  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (client.available()) {
        String data = client.readStringUntil('\n');  // Read incoming data
        Serial.println("Received: " + data);         // Print it to the serial monitor
      }
    }
    client.stop();  // Disconnect when done
    Serial.println("Client disconnected");
  }
}