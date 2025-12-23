#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <pgmspace.h>
#include "display.h"

bool fetchWMATAPredictions();

// Shared data (declared, not defined)
extern JsonArrayConst cachedTrains;
