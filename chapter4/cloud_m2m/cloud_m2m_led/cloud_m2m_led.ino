// Import required libraries
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <aREST.h>

// Status
int status = WL_IDLE_STATUS;

// WiFi parameters
const char* ssid     = "wifi-name";
const char* password = "wifi-pass";

// Clients
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Create aREST instance
aREST rest = aREST(client);

// Variables to be exposed to the API
bool ledState = false;

// Function
int ledControl(String command);
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Function to be exposed
  rest.function("toggle", ledToggle);

  // Give name and ID to device
  rest.set_id("305eyf");
  rest.set_name("mkr_led");

  // Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("WiFi connected");

  // Pin 6 as output
  pinMode(6, OUTPUT);
  
}

void loop() {

  // Connect to the cloud
  rest.handle(client);

}

// Custom function accessible by the API
int ledToggle(String command) {

  ledState = !ledState;

  digitalWrite(6, ledState);
  return 1;
}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
