/*
 * Simple VFD Display Example
 * 
 * Basic example showing how to use the MAX6921_VFD_Driver library
 * to display text and numbers on a 7BT317NK VFD display.
 * 
 * Hardware Setup - MAX6921 to Arduino Connections:
 * 
 * MAX6921 #1 (Controls Grids G0-G6 + Segments P0-P12):
 * ┌─────────────────────────────────────────────────────────┐
 * │ MAX6921 Pin │ Function │ Arduino Pin │ Description      │
 * │─────────────│──────────│─────────────│──────────────────│
 * │ Pin 20 (DIN)│ Data In  │ D11 (MOSI)  │ SPI Data Input   │
 * │ Pin 21 (CLK)│ Clock    │ D13 (SCK)   │ SPI Clock        │
 * │ Pin 22 (LOAD)│ Latch   │ D10 (SS)    │ Data Latch       │
 * │ Pin 23 (BLANK)│ Blank  │ D9          │ Display Enable   │
 * └─────────────────────────────────────────────────────────┘
 * 
 * MAX6921 #2 (Controls Segments P13-P20):
 * ┌─────────────────────────────────────────────────────────┐
 * │ MAX6921 Pin │ Function │ Arduino Pin │ Description      │
 * │─────────────│──────────│─────────────│──────────────────│
 * │ Pin 20 (DIN)│ Data In  │ D11 (MOSI)  │ SPI Data Input   │
 * │ Pin 21 (CLK)│ Clock    │ D13 (SCK)   │ SPI Clock        │
 * │ Pin 22 (LOAD)│ Latch   │ D8          │ Data Latch       │
 * │ Pin 23 (BLANK)│ Blank  │ D7          │ Display Enable   │
 * └─────────────────────────────────────────────────────────┘
 * 
 * IMPORTANT: DIN and CLK pins are shared between both MAX6921 chips!
 * Only LOAD and BLANK pins are separate for each chip.
 * 
 * Author: Your Name
 * Date: August 2025
 */

#include <MAX6921_VFD_Driver.h>

// Create VFD driver instance with specified pins
// Pin Configuration:
// - DIN (Data):  D11 (MOSI) - shared between both MAX6921 chips
// - CLK (Clock): D13 (SCK)  - shared between both MAX6921 chips  
// - MAX6921 #1 LOAD:  D10 (SS)   - Controls Grids + Segments P0-P12
// - MAX6921 #1 BLANK: D9         - Display enable for chip #1
// - MAX6921 #2 LOAD:  D8         - Controls Segments P13-P20
// - MAX6921 #2 BLANK: D7         - Display enable for chip #2
MAX6921_VFD_Driver vfd(10, 9, 8, 7);  // load1, blank1, load2, blank2

void setup() {
  Serial.begin(115200);
  Serial.println("MAX6921 VFD Driver - Simple Example");
  Serial.println("===================================");
  Serial.println("Pin Connections:");
  Serial.println("  DIN (Data):      D11 (MOSI)");
  Serial.println("  CLK (Clock):     D13 (SCK)");
  Serial.println("  MAX6921 #1 LOAD: D10 (SS)");
  Serial.println("  MAX6921 #1 BLANK:D9");
  Serial.println("  MAX6921 #2 LOAD: D8");
  Serial.println("  MAX6921 #2 BLANK:D7");
  Serial.println("===================================");
  
  // Initialize the VFD driver
  if (vfd.begin()) {
    Serial.println("VFD driver initialized successfully!");
  } else {
    Serial.println("Failed to initialize VFD driver!");
    while (1); // Stop execution
  }
  
  // Clear display
  vfd.clear();
  
  // Set brightness (0-255)
  vfd.setBrightness(200);
  
  // Display welcome message
  vfd.displayString("HELLO");
  delay(2000);
  
  Serial.println("Setup complete!");
}

void loop() {
  // Must call refresh() regularly to update the display
  vfd.refresh();
  
  // Demo sequence
  static unsigned long lastUpdate = 0;
  static uint8_t demoStep = 0;
  
  if (millis() - lastUpdate > 2000) { // Update every 2 seconds
    switch (demoStep) {
      case 0:
        vfd.displayString("VFD");
        Serial.println("Displaying: VFD");
        break;
        
      case 1:
        vfd.displayNumber(1234);
        Serial.println("Displaying: 1234");
        break;
        
      case 2:
        vfd.displayNumber(-567);
        Serial.println("Displaying: -567");
        break;
        
      case 3:
        vfd.displayString("TEST");
        Serial.println("Displaying: TEST");
        break;
        
      case 4:
        vfd.displayNumber(0);
        Serial.println("Displaying: 0");
        break;
        
      default:
        demoStep = 0;
        continue;
    }
    
    demoStep++;
    if (demoStep > 4) demoStep = 0;
    lastUpdate = millis();
  }
}
