#include "wmata.h"



#define CONFIG_URL "https://johnmihal.com/subway-display-config.json"
String WMATA_API_KEY = "";
const char* STATION_CODE = "C04";  // Foggy bottom

// ===== GLOBAL STORAGE (defined once) =====
StaticJsonDocument<4096> wmataDoc;
JsonArrayConst cachedTrains;

// ========================================

void getApiKey() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  HTTPClient http;
  http.begin(CONFIG_URL);
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    String payload = http.getString();
    
    // Parse JSON
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      String newKey = doc["wmata_api_key"].as<String>();
      

      Serial.println("API key detected!");
        
      WMATA_API_KEY = newKey;
      // prefs.begin("config", false);
      // prefs.putString("apikey", WMATA_API_KEY);
      // prefs.end();
        
      Serial.println("API key updated successfully!");
      Serial.println(WMATA_API_KEY);
    } else {
      Serial.println("Config checked - no changes");
    }
    
  } else {
    Serial.printf("API Key not reached, check network: %d\n", httpCode);
  }
  
  http.end();
}


bool fetchWMATAPredictions() {
  cachedTrains = JsonArrayConst(); // clear cache

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  String url = "http://api.wmata.com/StationPrediction.svc/json/GetPrediction/";
  url += STATION_CODE;

  // WiFiClientSecure client;
  // client.setCACert(WMATA_ROOT_CA);
  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  http.addHeader("api_key", WMATA_API_KEY);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP error: %d\n", httpCode);
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  wmataDoc.clear();
  DeserializationError error = deserializeJson(wmataDoc, payload);
  if (error) {
    Serial.println("JSON parse failed");
    return false;
  }

  cachedTrains = wmataDoc["Trains"].as<JsonArrayConst>();
  if (cachedTrains.isNull() || cachedTrains.size() == 0) {
    Serial.println("No trains");
    return false;
  }

  return true;
}
