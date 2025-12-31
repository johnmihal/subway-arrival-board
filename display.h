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

void writeDigit2(int device, int digit, byte value);
void displayString2(String text);

void writeRegisterAll(byte reg, byte data);
void setupDisplayAll();
void clearDisplayAll();
void writeRegisterDevice(int device, byte reg, byte data);
void displayStringAll(String text);