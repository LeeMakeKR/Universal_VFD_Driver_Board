/*
 * TEST.ino
 * 
 * MAX6921 데이지 체인 (2개 칩) 순차 비트 상승 테스트
 * 1비트씩 상승: 앞쪽 20비트 완료 후 뒤쪽 20비트 상승
 * 
 * 하드웨어 연결:
 * - Arduino D11  → MAX6921 DIN
 * - Arduino D13 → MAX6921 CLK  
 * - Arduino D10 → MAX6921 LOAD
 * - Arduino D9 → MAX6921 BLANK
 * 
 * 데이지 체인: MAX6921 #1 DOUT → MAX6921 #2 DIN
 */

/*
// 기존 코드 주석 처리
#include <Arduino.h>
#include <SPI.h>
#include "MAX6921_VFD_Driver.h"
#include "font.h"

// 핀 정의
const int DIN_PIN = 11;
const int CLK_PIN = 13;
const int LOAD_PIN = 10;
const int BLANK_PIN = 9;

// 순차 비트 상승 테스트 변수
uint32_t chip1_pattern = 0b00000000000000111111;
uint32_t chip2_pattern = 0b00000000000000000000;
int currentBitPosition = 6;
unsigned long lastToggle = 0;
const int TOGGLE_DELAY = 100;

MAX6921_VFD_Driver vfd;

void setup() {
  Serial.begin(9600);
  vfd.begin();
  vfd.clear();
  
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(BLANK_PIN, OUTPUT);
  
  digitalWrite(DIN_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(LOAD_PIN, HIGH);
  digitalWrite(BLANK_PIN, LOW);
}

void loop() {
  if (millis() - lastToggle >= TOGGLE_DELAY) {
    sendAllData(chip1_pattern, chip2_pattern);
    
    if (currentBitPosition < 20) {
      chip1_pattern |= (1UL << currentBitPosition);
    } else if (currentBitPosition < 28) {
      int chip2_bit = currentBitPosition - 20;
      chip2_pattern |= (1UL << chip2_bit);
    } else {
      chip1_pattern = 0b00000000000000111111;
      chip2_pattern = 0b00000000000000000000;
      currentBitPosition = 6;
      lastToggle = millis();
      return;
    }
    
    currentBitPosition++;
    lastToggle = millis();
  }
  
  if (Serial.available()) {
    String inputText = Serial.readString();
    inputText.trim();
    
    vfd.clear();
    vfd.displayText(inputText, 0);
    vfd.update();
  }
}

void sendAllData(uint32_t chip1_data, uint32_t chip2_data) {
  digitalWrite(LOAD_PIN, LOW);
  delayMicroseconds(1);
  
  for (int i = 19; i >= 0; i--) {
    bool bit = (chip2_data >> i) & 1;
    digitalWrite(DIN_PIN, bit);
    
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(2);
  }
  
  for (int i = 19; i >= 0; i--) {
    bool bit = (chip1_data >> i) & 1;
    digitalWrite(DIN_PIN, bit);
    
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(2);
  }
  
  digitalWrite(LOAD_PIN, HIGH);
  delayMicroseconds(5);
}
*/

#include <Arduino.h>
#include <SPI.h>
#include "VFD_7BT317NK_Config.h"
#include "MAX6921_VFD_Driver.h"
#include "VFD_7BT317NK_Font.h"

// Pin assignments for this specific hardware setup
#define DEFAULT_LOAD_PIN    10   // Common LOAD pin for all MAX6921 chips
#define DEFAULT_BLANK_PIN   9    // Common BLANK pin for all MAX6921 chips

MAX6921_VFD_Driver vfd(DEFAULT_LOAD_PIN, DEFAULT_BLANK_PIN, 
                       VFD_NUM_GRIDS, VFD_NUM_SEGMENTS, VFD_MAX_BRIGHTNESS);

void setup() {
  Serial.begin(9600);
  Serial.println("=== MAX6921 VFD Driver Test ===");
  
  vfd.begin();
  vfd.clear();
  
  Serial.println("텍스트를 입력하세요:");
}

void loop() {
  if (Serial.available()) {
    String text = Serial.readString();
    text.trim();
    
    Serial.print("표시: ");
    Serial.println(text);
    
    vfd.clear();
    vfd.displayString(text);
    vfd.refresh();
  }
}
