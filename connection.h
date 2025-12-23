#pragma once
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <time.h>

void setupConnection();
void syncTime();