#include "RunningMedian.h"
const int TEMP_SENSOR_PIN = A1;
int value;
unsigned long currTime;

// MEDIAN FOR TEMPS //
RunningMedian t_samples = RunningMedian(10); // reset count every 10 data points

int currentTemp;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(20);
  }

  pinMode(TEMP_SENSOR_PIN, INPUT);
}

void loop() {
  currTime = millis();
  tempLogger();
}

/////////////////////////////////////////////////////////////////
// Temperature Management ///////////////////////////////////////
/////////////////////////////////////////////////////////////////

int t_count = 0;
unsigned long temp_count_next = 0;

// READ TEMP
int readTemp(){
  int reading;
  reading = analogRead(TEMP_SENSOR_PIN); // 12 bit resolution
  return reading;
}

// UPDATES CURRENT TEMP
void tempLogger(){
  if (currTime > temp_count_next) {
    t_samples.add(readTemp());
    t_count++;
    temp_count_next = currTime + 100;
    if (t_count == 10) {
       currentTemp = t_samples.getMedian();
       Serial.println(currentTemp);
       t_count = 0;
    }
  }
}
