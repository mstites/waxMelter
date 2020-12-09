/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-thermostat-web-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
********/

// HEAT -> https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>


// PASSED FROM REST OF CODE
int currentTemp = 0;            // current temperature
int targetTemp = 130;           // target temperature
int flipOffset = 2;             // temperature offset to flip power on/off

boolean heating = false;        // heating



// Network information
const char* ssid = "GoofZone";
const char* password = "9139122626";

// Default Threshold Temperature Value
String inputMessage = String(targetTemp);
String lastTemperature;

// HTML web page to handle 2 input fields (threshold_input, enable_arm_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Wax Melter Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2>Status</h2> 
  <h3>Status: TODO </h3>
  <h3>Current Temperature: %CURR_TEMP% &deg;F</h3>
  <h3>Target Temperature: %TARG_TEMP% &deg;F</h3>
  <h2>Controls</h2>
  <form action="/get">
    Enter new target: <input type="number" step="1" name="threshold_input" value="%NEW_TARGET%" required>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

// Assign HTML placeholder values vars
String processor(const String& var){
  //Serial.println(var);
  if(var == "CURR_TEMP"){
    return String(currentTemp);
  }
  else if(var == "TARG_TEMP"){
    return String(targetTemp);
  }
  else if(var == "NEW_TARGET"){
    return inputMessage;
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Receive an HTTP GET request at <ESP_IP>/get?threshold_input=<inputMessage>&enable_arm_input=<inputMessage2>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
}
