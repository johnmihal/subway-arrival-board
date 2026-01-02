#include <Arduino.h>
#include "connection.h"
#include "display.h"
#include "wmata.h"
#include <cstring>

void setup() {
  Serial.begin(115200);

  setupConnection();

  // setupDisplay();

  // String text = "hello";

  // displayString2(text);

  getApiKey();

  setupDisplayAll();

  String text = "efghijklmnopqrstuvwxyz";

  displayStringAll(text);

  Serial.println("done");

}

// Globals
size_t trainIndex = 0;           // Which train to show next
unsigned long lastDisplayUpdate = -5000; 
unsigned long lastFetch = -300000;
String display_text = "";
int display_text_idx = 0;

int display_idx_1 = 3;
int display_idx_2 = 3;
int display_idx_3 = 3;

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

void display(){
  if (millis() - lastDisplayUpdate >= 2000 && cachedTrains.size() > 0) {
    display_text = "";
    lastDisplayUpdate = millis();

    if (cachedTrains.size() >= 1){
      JsonObjectConst train = cachedTrains[0];
      const char* line = train["Line"] | "   ";
      const char* dest = train["DestinationName"] | "   ";
      const char* min  = train["Min"] | " ";

      if (display_idx_1 >= strlen(dest)){
        display_idx_1 = 3;
      } else {
        display_idx_1++;
      }

      if (strcmp(min, "BRD") == 0 || strcmp(min, "ARR") == 0) {
        min = "0";
      }

      Serial.println(String(line) + " " + dest + " " + min);
      if (strlen(min) > 1){
        display_text += (String(line[0]) + " " + dest[display_idx_1-3] + dest[display_idx_1-2] + dest[display_idx_1-1] + dest[display_idx_1] + min);
      } else {
        display_text += (String(line[0]) + " " + dest[display_idx_1-3] + dest[display_idx_1-2] + dest[display_idx_1-1] + dest[display_idx_1] + " " + min);
      }
    } else {
      display_text += "No Train";
    }

    if (cachedTrains.size() >= 2 ){
      JsonObjectConst train = cachedTrains[1];
      const char* line = train["Line"] | "";
      const char* dest = train["DestinationName"] | "";
      const char* min  = train["Min"] | "";


      if (display_idx_2 >= strlen(dest)){
        display_idx_2 = 3;
      } else {
        display_idx_2++;
      }

      if (strcmp(min, "BRD") == 0 || strcmp(min, "ARR") == 0) {
        min = "0";
      }


      Serial.println(String(line) + " " + dest + " " + min);
      if (strlen(min) > 1){
        display_text += (String(line[0]) + " " + dest[display_idx_2-3] + dest[display_idx_2-2] + dest[display_idx_2-1] + dest[display_idx_2] + min);
      } else {
        display_text += (String(line[0]) + " " + dest[display_idx_2-3] + dest[display_idx_2-2] + dest[display_idx_2-1] + dest[display_idx_2] + " " + min);
      }

    } else {
      display_text += "        ";
    }

    if (cachedTrains.size() >= 3){
      JsonObjectConst train = cachedTrains[2];
      const char* line = train["Line"] | "";
      const char* dest = train["DestinationName"] | "";
      const char* min  = train["Min"] | "";


      if (display_idx_3 >= strlen(dest)){
        display_idx_3 = 3;
      } else {
        display_idx_3++;
      }

      if (strcmp(min, "BRD") == 0 || strcmp(min, "ARR") == 0) {
        min = "0";
      }


      Serial.println(String(line) + " " + dest + " " + min);
      if (strlen(min) > 1){
        display_text += (String(line[0]) + " " + dest[display_idx_3-3] + dest[display_idx_3-2] + dest[display_idx_3-1] + dest[display_idx_3] + min);
      } else {
        display_text += (String(line[0]) + " " + dest[display_idx_3-3] + dest[display_idx_3-2] + dest[display_idx_3-1] + dest[display_idx_3] + " " + min);
      }

    } else {
      display_text += "        ";
    }
    Serial.println(display_text);

    displayStringAll(display_text);
    
  }
}


void displayNew(){
  if (millis() - lastDisplayUpdate >= 3000 && cachedTrains.size() > 0) {
    display_text = "";
    lastDisplayUpdate = millis();

    for (size_t i = 0; i < cachedTrains.size(); ++i){
      JsonObjectConst train = cachedTrains[i];

      const char* line = train["Line"] | "";
      const char* dest = train["DestinationName"] | "";
      const char* min  = train["Min"] | "";


      if (display_idx_3 >= strlen(dest)){
        display_idx_3 = 3;
      } else {
        display_idx_3++;
      }

      if (strcmp(min, "BRD") == 0 || strcmp(min, "ARR") == 0) {
        min = "0";
      }

      display_text += (String(line) + " " + dest + " " + min + " ");
    }

    if (display_text.length() - display_text_idx < 24){
      display_text_idx = 0;
    } else {
      display_text = display_text.substring(display_text_idx);
      display_text_idx++;
    }

    Serial.println(display_text);

    displayStringAll(display_text);
      
  }
}

void loop() {
  fetcher();
  display();
}

