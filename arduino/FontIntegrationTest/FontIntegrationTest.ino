/*
 * Font Integration Verification
 * 
 * Simple test to verify that the VFD_7BT317NK_Font library
 * is properly integrated with the MAX6921_VFD_Driver library.
 * 
 * This test will print font patterns to Serial for verification.
 */

#include <MAX6921_VFD_Driver.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial on some boards
  
  Serial.println("=== Font Integration Test ===");
  Serial.println();
  
  // Test character pattern retrieval
  Serial.println("Testing character pattern retrieval:");
  
  // Test digits
  Serial.println("\nDigits (0-9):");
  for (char c = '0'; c <= '9'; c++) {
    uint32_t pattern = getVFDCharacterPattern(c);
    Serial.print("Character '");
    Serial.print(c);
    Serial.print("': 0b");
    Serial.println(pattern, BIN);
  }
  
  // Test letters
  Serial.println("\nLetters (A-Z):");
  for (char c = 'A'; c <= 'Z'; c++) {
    uint32_t pattern = getVFDCharacterPattern(c);
    Serial.print("Character '");
    Serial.print(c);
    Serial.print("': 0b");
    Serial.println(pattern, BIN);
  }
  
  // Test special characters
  Serial.println("\nSpecial Characters:");
  char special[] = {'+', '-', '*', '/', ':', '_', '.', ' '};
  for (int i = 0; i < 8; i++) {
    char c = special[i];
    uint32_t pattern = getVFDCharacterPattern(c);
    Serial.print("Character '");
    Serial.print(c);
    Serial.print("': 0b");
    Serial.println(pattern, BIN);
  }
  
  // Test font table info
  Serial.println("\nFont Table Information:");
  Serial.print("Total characters available: ");
  Serial.println(getVFDFontTableSize());
  
  // Test unknown character
  Serial.println("\nTesting unknown character:");
  uint32_t unknownPattern = getVFDCharacterPattern('?');
  Serial.print("Character '?': 0b");
  Serial.println(unknownPattern, BIN);
  
  Serial.println("\n=== Test Complete ===");
  Serial.println("If patterns are displayed above, font integration is working!");
}

void loop() {
  // Nothing to do in loop
}
