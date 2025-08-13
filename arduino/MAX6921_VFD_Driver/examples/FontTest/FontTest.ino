/*
 * Font Test Example
 * 
 * This example tests the VFD_7BT317NK_Font library integration
 * with the MAX6921_VFD_Driver library.
 * 
 * It will cycle through all available characters and display
 * their patterns for verification.
 * 
 * Hardware Setup:
 * See SimpleDisplay example for detailed pin connections.
 */

#include <MAX6921_VFD_Driver.h>

// Pin definitions (adjust as needed)
#define MAX6921_1_LOAD_PIN    10  // LOAD pin for MAX6921 #1
#define MAX6921_1_BLANK_PIN   9   // BLANK pin for MAX6921 #1
#define MAX6921_2_LOAD_PIN    8   // LOAD pin for MAX6921 #2
#define MAX6921_2_BLANK_PIN   7   // BLANK pin for MAX6921 #2

// Create VFD driver instance
MAX6921_VFD_Driver vfd(MAX6921_1_LOAD_PIN, MAX6921_1_BLANK_PIN,
                       MAX6921_2_LOAD_PIN, MAX6921_2_BLANK_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("=== VFD Font Test ===");
  Serial.println("Testing VFD_7BT317NK_Font integration");
  
  // Initialize VFD
  vfd.begin();
  vfd.setBrightness(128); // Medium brightness
  
  Serial.println("VFD Driver initialized");
  Serial.println("Starting font test...");
  
  // Test all digits
  testDigits();
  delay(2000);
  
  // Test all letters
  testLetters();
  delay(2000);
  
  // Test special characters
  testSpecialChars();
  delay(2000);
  
  Serial.println("Font test complete!");
}

void loop() {
  // Display a demo message
  vfd.displayString("DEMO");
  delay(2000);
  
  vfd.displayString("1234");
  delay(2000);
  
  vfd.displayString("ABCD");
  delay(2000);
  
  vfd.displayString("TEST");
  delay(2000);
}

void testDigits() {
  Serial.println("\n--- Testing Digits (0-9) ---");
  
  for (char digit = '0'; digit <= '9'; digit++) {
    Serial.print("Testing digit: ");
    Serial.println(digit);
    
    // Display single digit in all positions
    for (int pos = 0; pos < 4; pos++) {
      vfd.clear();
      vfd.displayCharacter(pos, digit);
      delay(500);
    }
    
    // Display digit repeated across display
    String testStr = "";
    testStr += digit;
    testStr += digit;
    testStr += digit;
    testStr += digit;
    
    vfd.displayString(testStr.c_str());
    delay(1000);
  }
}

void testLetters() {
  Serial.println("\n--- Testing Letters (A-Z) ---");
  
  for (char letter = 'A'; letter <= 'Z'; letter++) {
    Serial.print("Testing letter: ");
    Serial.println(letter);
    
    // Display single letter in all positions
    for (int pos = 0; pos < 4; pos++) {
      vfd.clear();
      vfd.displayCharacter(pos, letter);
      delay(300);
    }
    
    // Display letter repeated across display
    String testStr = "";
    testStr += letter;
    testStr += letter;
    testStr += letter;
    testStr += letter;
    
    vfd.displayString(testStr.c_str());
    delay(800);
  }
}

void testSpecialChars() {
  Serial.println("\n--- Testing Special Characters ---");
  
  char specialChars[] = {'+', '-', '*', '/', ':', '_', '.', ' '};
  int numSpecial = sizeof(specialChars) / sizeof(specialChars[0]);
  
  for (int i = 0; i < numSpecial; i++) {
    char ch = specialChars[i];
    Serial.print("Testing special char: '");
    Serial.print(ch);
    Serial.println("'");
    
    // Display single character in all positions
    for (int pos = 0; pos < 4; pos++) {
      vfd.clear();
      vfd.displayCharacter(pos, ch);
      delay(500);
    }
    
    // Display character repeated across display
    String testStr = "";
    testStr += ch;
    testStr += ch;
    testStr += ch;
    testStr += ch;
    
    vfd.displayString(testStr.c_str());
    delay(1000);
  }
}
