/*
 * VFD Display Test - Comprehensive Testing Example
 * 
 * This example provides comprehensive testing functions for the
 * 7BT317NK VFD display with MAX6921 drivers.
 * 
 * Test Functions:
 * - Grid scan test
 * - Segment scan test
 * - All segments on/off test
 * - Character display test
 * - Brightness test
 * 
 * Hardware Setup:
 * - Connect according to the pin mapping documentation
 * 
 * Serial Commands:
 * - 't' : Run all tests
 * - 'g' : Grid scan test
 * - 's' : Segment scan test
 * - 'a' : All segments test
 * - 'c' : Character test
 * - 'b' : Brightness test
 * - 'r' : Clear display
 * 
 * Author: Your Name
 * Date: August 2025
 */

#include <MAX6921_VFD_Driver.h>

// Create VFD driver instance
MAX6921_VFD_Driver vfd;

void setup() {
  Serial.begin(115200);
  Serial.println("=================================");
  Serial.println("7BT317NK VFD Display Test Suite");
  Serial.println("=================================");
  
  // Initialize VFD driver
  if (vfd.begin()) {
    Serial.println("✓ VFD driver initialized");
  } else {
    Serial.println("✗ Failed to initialize VFD driver");
    while (1);
  }
  
  // Initial setup
  vfd.clear();
  vfd.setBrightness(255);
  
  Serial.println("\nTest Commands:");
  Serial.println("  't' - Run all tests");
  Serial.println("  'g' - Grid scan test");
  Serial.println("  's' - Segment scan test");
  Serial.println("  'a' - All segments test");
  Serial.println("  'c' - Character test");
  Serial.println("  'b' - Brightness test");
  Serial.println("  'r' - Clear display");
  Serial.println("  'h' - Show this help");
  Serial.println("\nReady for commands...");
}

void loop() {
  // Continuously refresh display
  vfd.refresh();
  
  // Check for serial commands
  if (Serial.available()) {
    char command = Serial.read();
    processCommand(command);
  }
}

void processCommand(char command) {
  switch (command) {
    case 't':
    case 'T':
      runAllTests();
      break;
      
    case 'g':
    case 'G':
      gridScanTest();
      break;
      
    case 's':
    case 'S':
      segmentScanTest();
      break;
      
    case 'a':
    case 'A':
      allSegmentsTest();
      break;
      
    case 'c':
    case 'C':
      characterTest();
      break;
      
    case 'b':
    case 'B':
      brightnessTest();
      break;
      
    case 'r':
    case 'R':
      vfd.clear();
      Serial.println("Display cleared");
      break;
      
    case 'h':
    case 'H':
      showHelp();
      break;
      
    default:
      Serial.println("Unknown command. Type 'h' for help.");
      break;
  }
}

void runAllTests() {
  Serial.println("\n>>> Running All Tests <<<");
  
  allSegmentsTest();
  delay(1000);
  
  gridScanTest();
  delay(1000);
  
  segmentScanTest();
  delay(1000);
  
  characterTest();
  delay(1000);
  
  brightnessTest();
  
  vfd.clear();
  Serial.println("All tests completed!");
}

void gridScanTest() {
  Serial.println("\n--- Grid Scan Test ---");
  Serial.println("Testing each grid individually...");
  
  vfd.clear();
  
  // Test each grid with all segments on
  for (int grid = 0; grid < VFD_NUM_GRIDS; grid++) {
    Serial.print("Grid ");
    Serial.print(grid);
    Serial.println(" ON");
    
    // Turn on all segments for current grid
    vfd.setGrid(grid, 0x1FFFFF); // All 21 segments
    
    delay(500);
    
    // Turn off current grid
    vfd.setGrid(grid, 0);
  }
  
  Serial.println("Grid scan test completed");
}

void segmentScanTest() {
  Serial.println("\n--- Segment Scan Test ---");
  Serial.println("Testing each segment individually...");
  
  vfd.clear();
  
  // Test each segment on all grids
  for (int segment = 0; segment < VFD_NUM_SEGMENTS; segment++) {
    Serial.print("Segment P");
    Serial.print(segment);
    Serial.println(" ON");
    
    // Turn on current segment on all grids
    uint32_t segmentMask = (1UL << segment);
    for (int grid = 0; grid < VFD_NUM_GRIDS; grid++) {
      vfd.setGrid(grid, segmentMask);
    }
    
    delay(200);
    
    // Turn off current segment
    for (int grid = 0; grid < VFD_NUM_GRIDS; grid++) {
      vfd.setGrid(grid, 0);
    }
  }
  
  Serial.println("Segment scan test completed");
}

void allSegmentsTest() {
  Serial.println("\n--- All Segments Test ---");
  
  // Turn on all segments
  Serial.println("All segments ON");
  for (int grid = 0; grid < VFD_NUM_GRIDS; grid++) {
    vfd.setGrid(grid, 0x1FFFFF); // All segments
  }
  
  delay(2000);
  
  // Turn off all segments
  Serial.println("All segments OFF");
  vfd.clear();
  
  delay(1000);
  
  // Blink test
  Serial.println("Blinking test...");
  for (int i = 0; i < 5; i++) {
    // All on
    for (int grid = 0; grid < VFD_NUM_GRIDS; grid++) {
      vfd.setGrid(grid, 0x1FFFFF);
    }
    delay(300);
    
    // All off
    vfd.clear();
    delay(300);
  }
  
  Serial.println("All segments test completed");
}

void characterTest() {
  Serial.println("\n--- Character Test ---");
  
  // Test numbers
  Serial.println("Testing numbers 0-9...");
  for (int i = 0; i <= 9; i++) {
    vfd.displayNumber(i);
    Serial.print(i);
    Serial.print(" ");
    delay(500);
  }
  Serial.println();
  
  delay(1000);
  
  // Test some letters
  Serial.println("Testing letters...");
  const char* testWords[] = {"HELP", "TEST", "GOOD", "FAIL"};
  const int numWords = sizeof(testWords) / sizeof(testWords[0]);
  
  for (int i = 0; i < numWords; i++) {
    vfd.displayString(testWords[i]);
    Serial.print(testWords[i]);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println();
  
  Serial.println("Character test completed");
}

void brightnessTest() {
  Serial.println("\n--- Brightness Test ---");
  
  vfd.displayString("BRIGHT");
  
  // Fade down
  Serial.println("Fading down...");
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    vfd.setBrightness(brightness);
    Serial.print("Brightness: ");
    Serial.println(brightness);
    delay(50);
  }
  
  delay(500);
  
  // Fade up
  Serial.println("Fading up...");
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    vfd.setBrightness(brightness);
    Serial.print("Brightness: ");
    Serial.println(brightness);
    delay(50);
  }
  
  // Set to normal brightness
  vfd.setBrightness(200);
  Serial.println("Brightness test completed");
}

void showHelp() {
  Serial.println("\n=== VFD Test Commands ===");
  Serial.println("  't' - Run all tests");
  Serial.println("  'g' - Grid scan test");
  Serial.println("  's' - Segment scan test");
  Serial.println("  'a' - All segments test");
  Serial.println("  'c' - Character test");
  Serial.println("  'b' - Brightness test");
  Serial.println("  'r' - Clear display");
  Serial.println("  'h' - Show this help");
  Serial.println("========================");
}
