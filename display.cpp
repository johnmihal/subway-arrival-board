#include "display.h"
// MAX7219 Display - Numbers and Text


#define DATA_PIN  13   // DIN
#define CLK_PIN   14   // CLK  
#define CS_PIN    15   // CS

// MAX7219 Registers
#define REG_DECODE_MODE 0x09
#define REG_INTENSITY   0x0A
#define REG_SCAN_LIMIT  0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAY_TEST 0x0F

// *** CHOOSE WHAT TO DISPLAY ***
// Option 1: Display a number
long numberToDisplay = 12345678;

// Option 2: Display text (8 characters max)
String textToDisplay = "HELLO   ";  // Pad with spaces for 8 chars

// 7-segment patterns for digits 0-9
const byte digits[10] = {
  0b01111110,  // 0
  0b00110000,  // 1
  0b01101101,  // 2
  0b01111001,  // 3
  0b00110011,  // 4
  0b01011011,  // 5
  0b01011111,  // 6
  0b01110000,  // 7
  0b01111111,  // 8
  0b01111011   // 9
};

// 7-segment patterns for letters and symbols
// Format: bit pattern for segments (DP-A-B-C-D-E-F-G)
byte getCharPattern(char c) {
  switch(toupper(c)) {
    // Numbers
    case '0': return 0b01111110;
    case '1': return 0b00110000;
    case '2': return 0b01101101;
    case '3': return 0b01111001;
    case '4': return 0b00110011;
    case '5': return 0b01011011;
    case '6': return 0b01011111;
    case '7': return 0b01110000;
    case '8': return 0b01111111;
    case '9': return 0b01111011;
    
    // Letters that look good on 7-segment
    case 'A': return 0b01110111;
    case 'B': return 0b00011111;  // lowercase b
    case 'C': return 0b01001110;
    case 'D': return 0b00111101;  // lowercase d
    case 'E': return 0b01001111;
    case 'F': return 0b01000111;
    case 'G': return 0b01011110;
    case 'H': return 0b00110111;
    case 'I': return 0b00110000;  // looks like 1
    case 'J': return 0b00111100;
    case 'K': return 0b10000111;
    case 'L': return 0b00001110;
    case 'M': return 0b10010101;
    case 'N': return 0b01110110;  // lowercase n
    case 'O': return 0b01111110;  // looks like 0
    case 'P': return 0b01100111;
    case 'Q': return 0b11110011;
    case 'R': return 0b00000101;  // lowercase r
    case 'S': return 0b01011011;  // looks like 5
    case 'T': return 0b01000110;  // lowercase t
    case 'U': return 0b00111110;
    case 'V': return 0b00111110;  // lowercase u
    case 'W': return 0b10011100;
    case 'X': return 0b10110001;
    case 'Y': return 0b00111011;
    case 'Z': return 0b01101101;  // looks like 2
    
    // Symbols
    case '-': return 0b00000001;  // minus/dash
    case '_': return 0b00001000;  // underscore
    case ' ': return 0b00000000;  // space (blank)
    case '=': return 0b00001001;  // equals
    case '*': return 0b01100011;  // degree symbol
    
    default:  return 0b00000000;  // blank for unknown chars
  }
}

void writeRegister(byte reg, byte data) {
  digitalWrite(CS_PIN, LOW);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, reg);
  shiftOut(DATA_PIN, CLK_PIN, MSBFIRST, data);
  digitalWrite(CS_PIN, HIGH);
}

void clearDisplay() {
  for (int i = 1; i <= 8; i++) {
    writeRegister(i, 0x00);
  }
}

void displayNumber(long num) {
  clearDisplay();
  Serial.print("Displaying number: ");
  Serial.println(num);
  
  bool isNegative = false;
  if (num < 0) {
    isNegative = true;
    num = abs(num);
  }
  
  if (num == 0) {
    writeRegister(1, digits[0]);
    return;
  }
  
  int position = 1;
  while (num > 0 && position <= 8) {
    int digit = num % 10;
    writeRegister(position, digits[digit]);
    num /= 10;
    position++;
  }
  
  if (isNegative && position <= 8) {
    writeRegister(position, 0b00000001);
  }
}

void displayString(String text) {
  clearDisplay();
  Serial.print("Displaying text: ");
  Serial.println(text);
  
  // Ensure text is max 8 characters
  if (text.length() > 8) {
    text = text.substring(0, 8);
  }
  
  // Display from right to left
  for (int i = 0; i < text.length() && i < 8; i++) {
    int position = i + 1;  // Position 1-8
    char c = text[text.length() - 1 - i];  // Read string backwards
    writeRegister(position, getCharPattern(c));
  }
}

void displayScrollText(const String& text, int delayMs) {
  // Pad text so it scrolls in and out
  String padded = text + "        ";
  bool done = false;
  static unsigned long lastDisplay = 0;
  int i = 0;
  while (done != true){
    if (millis() - lastDisplay >= 1000){
      if(i <= padded.length() - 8) {
        String frame = padded.substring(i, i + 8);
        displayString(frame);
        if (i == padded.length()-9){
          done = true;
        }
      }
      lastDisplay = millis();
      i++;
    }
  }
}

void setupDisplay() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  
  // Initialize MAX7219
  writeRegister(REG_SHUTDOWN, 0x00);
  writeRegister(REG_DECODE_MODE, 0x00);
  writeRegister(REG_SCAN_LIMIT, 0x07);
  writeRegister(REG_INTENSITY, 0x08);  // Brightness 0-15
  writeRegister(REG_DISPLAY_TEST, 0x00);
  clearDisplay();
  writeRegister(REG_SHUTDOWN, 0x01);
}

#define NUM_DEVICES 3

void writeDigit2(int device, int digit, byte value) {
  digitalWrite(CS_PIN, LOW);

  for (int d = NUM_DEVICES - 1; d >= 0; d--) {
    if (d == device) {
      SPI.transfer(digit);
      SPI.transfer(value);
    } else {
      SPI.transfer(digit);
      SPI.transfer(0x00);
    }
  }

  digitalWrite(CS_PIN, HIGH);
}

void displayString2(String text) {
  clearDisplay();

  if (text.length() > 24) {
    text = text.substring(0, 24);
  }

  // Fill from right to left across all 24 digits
  for (int i = 0; i < text.length(); i++) {
    int globalPos = i;                  // 0–23
    int device = globalPos / 8;         // 0–2
    int digit  = (globalPos % 8) + 1;   // 1–8

    char c = text[text.length() - 1 - i];
    writeDigit2(device, digit, getCharPattern(c));
  }
}

void writeRegisterAll(byte reg, byte data) {
  digitalWrite(CS_PIN, LOW);
  for (int i = 0; i < 3; i++) {
    SPI.transfer(reg);
    SPI.transfer(data);
  }
  digitalWrite(CS_PIN, HIGH);
}

void setupDisplayAll() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  SPI.begin(CLK_PIN, -1, DATA_PIN, CS_PIN);

  writeRegisterAll(REG_SHUTDOWN, 0x00);
  writeRegisterAll(REG_DECODE_MODE, 0x00);
  writeRegisterAll(REG_SCAN_LIMIT, 0x07);
  writeRegisterAll(REG_INTENSITY, 0x08);
  writeRegisterAll(REG_DISPLAY_TEST, 0x00);
  clearDisplayAll();
  writeRegisterAll(REG_SHUTDOWN, 0x01);
}

void clearDisplayAll() {
  for (int digit = 1; digit <= 8; digit++) {
    writeRegisterAll(digit, 0x00);
  }
}

void writeRegisterDevice(int device, byte reg, byte data) {
  digitalWrite(CS_PIN, LOW);

  for (int d = 2; d >= 0; d--) {
    if (d == device) {
      SPI.transfer(reg);
      SPI.transfer(data);
    } else {
      SPI.transfer(0x00);  // NOOP
      SPI.transfer(0x00);
    }
  }

  digitalWrite(CS_PIN, HIGH);
}

void displayStringAll(String text) {
  clearDisplayAll();

  if (text.length() > 24) {
    text = text.substring(0, 24);
  }

  for (int i = 0; i < text.length(); i++) {
    char c = text[i];
    // char c = text[text.length() - 1 - i];

    byte pattern = getCharPattern(c);

    int device = i / 8;
    int digit  = ((i % 8) + 1 - 9) * (-1);

    writeRegisterDevice(device, digit, pattern);
  }
}

