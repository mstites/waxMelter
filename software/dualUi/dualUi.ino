/////////////////////////////////////////////////////////////////

#include "Button2.h";
#include "RunningMedian.h"

// OLED
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#define I2C_ADDRESS 0x3C // 7bit address, 8bit address is 0x78

// WEB UI
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

/////////////////////////////////////////////////////////////////
// Pin Assignments //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const int PLATE_POWER_PIN = A0;
const int TEMP_SENSOR_PIN = A2;

const int UP_BUTTON_PIN = A8;
const int SELECT_BUTTON_PIN = A9;
const int DOWN_BUTTON_PIN = A10;

/////////////////////////////////////////////////////////////////
// Instantiate objects  /////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// BUTTONS //
Button2 up_button = Button2(UP_BUTTON_PIN);
Button2 select_button = Button2(SELECT_BUTTON_PIN);
Button2 down_button = Button2(DOWN_BUTTON_PIN);

// OLED //
SSD1306AsciiWire oled;

// MEDIAN FOR TEMPS //
RunningMedian t_samples = RunningMedian(10); // reset count every 10 data points

/////////////////////////////////////////////////////////////////
// State Values /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// variables currentTemp
// line up comments
// mode -> had another part to it
// selection -> new variable name (selectedMenuItem)

int currentTemp = 0;            // current temperature
int prevTemp = 0;               // last current temp
int targetTemp = 130;           // target temperature
int flipOffset = 2;             // temperature offset to flip power on/off

boolean heating = false;        // heating
boolean platePower = false;     // pwr to relay controlling hotplate
boolean heated = false;         // at temp

byte buttonMode = 1;            // 1 -> select, 2 -> adjust target temp
int lineSelection = 1;          // line selected on OLED
byte currentScreen = 0;         // 0-STANDBY, 1-HEATING

unsigned long currTime;                  // milis 

/////////////////////////////////////////////////////////////////
// Network Info  ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

const char* ssid = "GoofZone";
const char* password = "9139122626";

/////////////////////////////////////////////////////////////////

void setup() {
  // open serial
  Serial.begin(9600);

  // configure pins
  pinMode(PLATE_POWER_PIN, OUTPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);

  // init display
  Wire.begin();
  Wire.setClock(400000l);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  standbyScreen(1, 1, currentTemp, targetTemp);

  // start web ui
  startWebUi();
//  startWifi();

  delay(1000);
  // set button handlers
  up_button.setTapHandler(upPress);
  select_button.setTapHandler(selectPress);
  down_button.setTapHandler(downPress);
}

/////////////////////////////////////////////////////////////////

void loop() {
//  Serial.println(analogRead(TEMP_SENSOR_PIN));
  // variables camel case
  
  currTime = millis();
  
  // initalize constant checking
  up_button.loop();
  select_button.loop();
  down_button.loop();

  tempLogger();
  controlPlate();
  updateOLED();
//  refreshScreen(0, currentTemp, targetTemp);

  // loop to update temperature
  // loop to manage power plate

  // global state value to track current state, can be updated by buttons?
  
}

/////////////////////////////////////////////////////////////////
// Button Loops /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void upPress(Button2& btn) {
    Serial.println("Up: Pressed");
    arrow(1);
}

void selectPress(Button2& btn) {
    unsigned int time = btn.wasPressedFor();
    if (time < 1000) {
        Serial.println("Select: Regular press");
        middle();
    } 
    else {
        Serial.println("Select: long press");
        Serial.println("Turning OFF");
        heating = false;
        platePower = false; // should be handled somewhere else
    }
}

void downPress(Button2& btn) {
    Serial.println("Down: Pressed");
    arrow(-1);
}

/////////////////////////////////////////////////////////////////
// Button Func/State Handler ////////////////////////////////////
/////////////////////////////////////////////////////////////////

// MIDDLE BUTTON
void middle(){
  if (buttonMode == 1){               // SELECT
    if (lineSelection == 1) {
       buttonMode = 2;                // change target temp
    }
    else if (currentScreen == 0) {
       changeScreen(1);               // go to heating screen
       heating = true; 
       heated = false;                // reset
    }
    else if (currentScreen == 1) {
       heating = false; 
       heated = false;                // reset
       changeScreen(0);               // go to heating screen     
    }
  }
  else if (buttonMode == 2){          // ADJUST
    buttonMode = 1;                   // back to select mode
  }
}

// BUTTOM/TOP BUTTON FUNCTIONALITY
void arrow(int dir){
  if (buttonMode == 1){               // SELECT
    refreshScreen(dir, currentTemp, targetTemp);
  }
  else if (buttonMode == 2){          // ADJUST
    targetTemp = targetTemp + dir;    // change target temp
    refreshScreen(0, currentTemp, targetTemp); // no selection change
  }
  Serial.println(lineSelection);
}

// CHANGE TO NEW SCREEN
void changeScreen(byte newScreen){
  if (currentScreen != newScreen) {
    currentScreen = newScreen;               // update global
    lineSelection = 1;                // reset counter
    refreshScreen(0, currentTemp, targetTemp);
  }
}

/////////////////////////////////////////////////////////////////
// OLED Display /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

unsigned long oled_next = 0;

void updateOLED() {
    // if within five of target temp, update every temp
    if ((abs(targetTemp - currentTemp) <= 5) && (currentTemp != prevTemp) { 
      refreshScreen(0, currentTemp, targetTemp);
      prevTemp = currentTemp;
    }
    else if (abs(currentTemp - prevTemp) >= 5) { // otherwise only for steps of 5
      refreshScreen(0, currentTemp, targetTemp);
      prevTemp = currentTemp;
    }
    if (currTime > oled_next) {
      oled_next = currTime + 500;
      checkHeatingStatus();
    }
}

void checkHeatingStatus() {
  // Checks if OLED is correct given status, and refreshes as needed
  // needed because of web UI
  if ((heating) && (currentScreen != 1)) {
    changeScreen(1);
  }
  else if (!(heating) && (currentScreen != 0)) {
    changeScreen(0);
  }
}

// WRITE SCREEN
void printScreen(String L1, String L2, String L3, String L4, String L5, String L6, String L7, String L8) {
    oled.clear();
    oled.println(L1);
    oled.println(L2);
    oled.println(L3);
    oled.println(L4);
    oled.println(L5);
    oled.println(L6);
    oled.println(L7);
    oled.println(L8);
}


// COMPUTE LOCATION (selction with limits)
int compSelection (int maxSel, int prevSel, int dir) {
    int sel;
    sel = prevSel - dir;
    if (sel > maxSel) {
      sel = maxSel;
    }
    else if (sel < 1) {
      sel = 1;
    }
    return(sel);
}

// REFRESH SCREEN
void refreshScreen(int dir, int temp, int target) { // should be done immediately
  if (currentScreen == 0) { // STANDBY
    lineSelection = standbyScreen(lineSelection, dir, temp, target);
  }
  else if (currentScreen == 1) { // heating
    lineSelection = heatingScreen(lineSelection, dir, temp, target);
  }
}

// STANDBY SCREEN - Screen 0
int standbyScreen(int prevSel, int dir, int temp, int target) {
    int maxSel = 2; 
    int selection = compSelection(maxSel, prevSel, dir);
    String arrow = String("`");
    String tempCurrent = String(temp);
    String tempTarget = String(target);
    String blank = String("");
    String L1 = String("STATUS: STANDBY");
    String L2 = String("WEB: " + String(WiFi.localIP().toString().c_str()));
    String L3 = String("Current temp      " + tempCurrent);
    String L4 = String("Temp target       " + tempTarget);
    if (selection == 1) {
        String L8 = String("Start heating       ");
        String L6 = String("Change Target Temp  " + arrow);
        printScreen(L1, L2, L3, L4, blank, L6, blank, L8);
    }
    else {
        String L8 = String("Start heating       " + arrow);
        String L6 = String("Change Target Temp  ");   
        printScreen(L1, L2, L3, L4, blank, L6, blank, L8);
    }
    return(selection);
}

// 1 SCREEN - Screen 1
int heatingScreen(int prevSel, int dir, int temp, int target) {
    int maxSel = 2; 
    int selection = compSelection(maxSel, prevSel, dir);
    String arrow = String("`");
    String tempCurrent = String(temp);
    String tempTarget = String(target);
    String blank = String("");
    String L1 = String("STATUS: HEATING");
    String L2 = String("WEB: " + String(WiFi.localIP().toString().c_str()));  
    String L3 = String("Current temp      " + tempCurrent);
    String L4 = String("Temp target       " + tempTarget);
    if (selection == 1) {
        String L6 = String("Change Target Temp  " + arrow);
        String L8 = String("Cancel/Exit         ");
        printScreen(L1, L2, L3, L4, blank, L6, blank, L8);
    }
    else {
        String L6 = String("Change Target Temp  ");
        String L8 = String("Cancel/Exit         " + arrow);
        printScreen(L1, L2, L3, L4, blank, L6, blank, L8);
    }
    return(selection);
}

/////////////////////////////////////////////////////////////////
// Temperature Management ///////////////////////////////////////
/////////////////////////////////////////////////////////////////

int t_count = 0;
unsigned long temp_count_next = 0;

// READ TEMP
int readTemp(){
//  float vOut;
//  vOut = analogRead(TEMP_SENSOR_PIN) * (3.3/4095); // 12 bit resolution
//  return vOut*362.44-353.23;
  return((0.2921*analogRead(TEMP_SENSOR_PIN)) - 353.233423);
}

// UPDATES CURRENT TEMP
void tempLogger(){
  if (currTime > temp_count_next) {
    t_samples.add(readTemp());
    t_count++;
    temp_count_next = currTime + 100;
    if (t_count == 10) {
       currentTemp = t_samples.getMedian();
       t_count = 0;
    }
  }
}

/////////////////////////////////////////////////////////////////
// Plate Control ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// CONTROL POWER VIA RELAY
void controlPlate() {
  if (heating){
    if ((currentTemp - targetTemp) >= flipOffset) {
      setPlatePower(false);
      heated = true;
    }
    else if (currentTemp <= (targetTemp - flipOffset)) {
      setPlatePower(true);
      heated = true;
    }
  }
  else if (platePower){ // off with plate on
      setPlatePower(false);
  }
}

// SET DESIRED STATE
void setPlatePower(boolean desired) {
   if (platePower == desired) {
       ; // do nothing
   }
   else if (desired == false){
    // set false, turn off 
    platePower = false;
    digitalWrite(PLATE_POWER_PIN, LOW);
   }
   else {
    // set true, turn on
    platePower = true;
    digitalWrite(PLATE_POWER_PIN, HIGH);    
   }
   Serial.print("Plate power: ");
   Serial.println(platePower);
}

/////////////////////////////////////////////////////////////////
// Web UI Values ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

AsyncWebServer server(80);

// Pointers to the field values
const char* PARAM_INPUT_1 = "targTempInput";
const char* PARAM_INPUT_2 = "state";

// HTML CODE
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Wax Melter Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="10">

  <!--STYLE INFO-->
  <style>
  html {font-family: Arial; display: inline-block; text-align: center;}
  h1 {font-size: 3.0rem;}
  h2 {font-size: 2.0rem;}
  h3 {font-weight: normal;}
  p {font-size: 1.0rem;}
  body {max-width: 600px; margin:0px auto; padding-bottom: 25px; padding-top: 25px;}
  .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
  .switch input {display: none}
  .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
  .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
  input:checked+.slider {background-color: #b30000}
  input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
  </head><body>

  <!--STATUS INDICATORS-->
  <h1>Wax Melter</h1> 
  <p style="font-style: italic;">Page refreshes every 10 seconds</p>
  <h2>Temp Status</h2>
  <h3>Current Temperature: %CURR_TEMP% &deg;F</h3>
  <h3>Target Temperature: %TARG_TEMP% &deg;F</h3>
  <h2>Control</h2>
  <!--TARGET TEMP-->
  <h3>New target: </h3>
    <form action="/update">
    <input type="number" step="1" name="targTempInput" value="%NEW_TARGET%" required>
    <input type="submit" value="Submit">
  </form>
  <!--BUTTON SLIDER-->
  %HEAT_SLIDER%
  <script>function toggleCheckbox(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
    else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
    xhr.send();
  }
  </script>
</body></html>)rawliteral";

/////////////////////////////////////////////////////////////////
// Web UI Support Functions /////////////////////////////////////
/////////////////////////////////////////////////////////////////

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Translate C variable to HTML button look
String outputState(){
  if(heating){
    return "checked";
  }
  else {
    return "";
  }
}

// Assign HTML placeholder values vars
String processor(const String& var){
  //Serial.println(var);
  if(var == "CURR_TEMP"){
    return String(currentTemp); // current temp status
  }
  else if(var == "TARG_TEMP"){
    return String(targetTemp); // target temp status
  }
  else if(var == "NEW_TARGET"){
    return String(targetTemp); // new target value in field
  }
  else if(var == "HEAT_SLIDER"){
    String buttons = "";
    buttons += "<h3>Heating:</h3><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState() + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

/////////////////////////////////////////////////////////////////
// Web UI Startup ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void startWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
}

void printAddress() {
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
}

void serverInit() {
  // DEFAULT BEHAVIOR
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", INDEX_HTML, processor);
  });

  // USER UPDATE BEHAVIOR
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT_1)) {           // if target temp change
      targetTemp = (request->getParam(PARAM_INPUT_1)->value()).toInt();
      Serial.println(String(targetTemp));
    }
    else if (request->hasParam(PARAM_INPUT_2)) {      // if heating status change
      int input = (request->getParam(PARAM_INPUT_2)->value()).toInt();
      if (input == 1){
        heating = true;
      }
      else {
        heating = false;
      }
      Serial.println(input);     
    }
    // Page after user inputs: 
    request->send(200, "text/html", "Request sent to your wax melter.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

char startWebUi() {
  startWifi();
  printAddress();
  serverInit();
}


/////////////////////////////////////////////////////////////////
