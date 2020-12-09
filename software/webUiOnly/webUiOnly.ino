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

  <!--STYLE INFO-->
  <style>
  html {font-family: Arial; display: inline-block; text-align: center;}
  h2 {font-size: 3.0rem;}
  p {font-size: 3.0rem;}
  body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
  .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
  .switch input {display: none}
  .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
  .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
  input:checked+.slider {background-color: #b30000}
  input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
  </head><body>

  <!--STATUS INDICATORS-->
  <h2>Status</h2> 
  <h3>Status: TODO </h3>
  <h3>Current Temperature: %CURR_TEMP% &deg;F</h3>
  <h3>Target Temperature: %TARG_TEMP% &deg;F</h3>
  <h2>Controls</h2>

  <!--TARGET TEMP-->
  <form action="/submit">
    Enter new target: <input type="number" step="1" name="threshold_input" value="%NEW_TARGET%" required>
    <input type="submit" value="Submit">
  </form>
  
  <!--BUTTON SLIDER-->
  %HEAT_SLIDER%
  <script>function toggleCheckbox(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/updateSlider?output="+element.id+"&state=1", true); }
    else { xhr.open("GET", "/updateSlider?output="+element.id+"&state=0", true); }
    xhr.send();
  }
  </script>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String outputState(){
  if(heating){
    return "checked";
  }
  else {
    return "";
  }
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
  else if(var == "HEAT_SLIDER"){
    String buttons = "";
    buttons += "<h4>HEAT</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" " + outputState() + "><span class=\"slider\"></span></label>";
    return buttons;
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
  server.on("/submit", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.on("/updateSlider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println(heating);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
}
