#include <Arduino.h>
#include "connection.h"
#include "display.h"
#include "wmata.h"

void setup() {
  Serial.begin(115200);

  setupConnection();

  // setupDisplay();

  // String text = "hello";

  // displayString2(text);

  // getApiKey();

  setupDisplayAll();

  // writeRegisterDevice(0, 1, 0xFF);
  // writeRegisterDevice(1, 1, 0x0F);
  // writeRegisterDevice(2, 1, 0xF0);

  // for (int d = 1; d <= 8; d++) {
  //   writeRegisterDevice(0, d, 0xFF);
  // }

  // for (int d = 1; d <= 8; d++) {
  //   writeRegisterDevice(1, d, 0x0F);
  // }

  // for (int d = 1; d <= 8; d++) {
  //   writeRegisterDevice(2, d, 0xF0);
  // }

  String text = "efghijklmnopqrstuvwxyz";

  displayStringAll(text);

  Serial.println("done");

}

// Globals
size_t trainIndex = 0;           // Which train to show next
unsigned long lastSwitch = 0; 
unsigned long lastFetch = 0;
String display_text = "";
int display_text_idx = 0;

void fetcher(){
  if (millis() - lastFetch >= 30000) {
    Serial.println("Fetching prediction");
    if (fetchWMATAPredictions()) {
      trainIndex = 0;
      Serial.println("Successfully got predictions");
      JsonVariantConst train = cachedTrains[trainIndex];
      serializeJson(train, Serial);
      Serial.println();
    } else{
      Serial.println("Failed to get predictions");
    }
    
    lastFetch = millis();
  }
}

void loop() {
  // fetcher();

  // while (trainIndex < cachedTrains.size()) {
  //   if (millis() - lastSwitch >= 5000 && cachedTrains.size() > 0) {
  //     lastSwitch = millis();

  //     JsonObjectConst train = cachedTrains[trainIndex];

  //     const char* line = train["Line"] | "";
  //     const char* dest = train["DestinationName"] | "";
  //     const char* min  = train["Min"] | "";


  //     Serial.println(String(line) + " " + dest + " " + min);

  //     displayScrollText(String(line) + " " + dest + " " + min);
  //     trainIndex = trainIndex + 1;
  //   }
  // }
}

