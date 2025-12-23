#pragma once
#include <Arduino.h>

#include <SPI.h>

byte getCharPattern(char c);
void writeRegister(byte reg, byte data);
void clearDisplay();
void displayNumber(long num);
void displayString(String text);
void setupDisplay();
void displayScrollText(const String& text, int delayMs = 250);