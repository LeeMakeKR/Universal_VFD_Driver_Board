/*
 * MAX6921_VFD_Driver.h
 * 
 * Arduino library for controlling VFD displays using MAX6921 driver chips
 * Specifically designed for 7BT317NK VFD display
 * 
 * Hardware Requirements:
 * - MAX6921 VFD driver IC(s)
 * - Compatible VFD display (tested with 7BT317NK)
 * - Arduino Uno/Nano/Pro Mini or compatible
 * 
 * Pin Connections:
 * See example sketch for detailed pin mapping
 * 
 * Author: Your Name
 * Date: August 2025
 * Version: 1.0
 */

#ifndef MAX6921_VFD_DRIVER_H
#define MAX6921_VFD_DRIVER_H

#include <Arduino.h>
#include <SPI.h>
#include "../VFD_7BT317NK_Font/VFD_7BT317NK_Font.h"

// Library version
#define MAX6921_VFD_DRIVER_VERSION "1.0.0"

// Default pin assignments (can be overridden in constructor)
#define DEFAULT_MAX6921_1_LOAD_PIN    10
#define DEFAULT_MAX6921_1_BLANK_PIN   9
#define DEFAULT_MAX6921_2_LOAD_PIN    8
#define DEFAULT_MAX6921_2_BLANK_PIN   7

// Display specifications for 7BT317NK
#define VFD_NUM_GRIDS      7     // G0-G6
#define VFD_NUM_SEGMENTS   21    // P0-P20
#define VFD_NUM_DIGITS     4     // 4-digit display
#define VFD_MAX_BRIGHTNESS 255   // Maximum brightness level

// Timing constants
#define DEFAULT_GRID_SCAN_DELAY_US  2000  // Microseconds per grid
#define DEFAULT_SPI_CLOCK_SPEED     4000000  // 4MHz SPI clock

class MAX6921_VFD_Driver {
private:
    // Hardware pin assignments
    uint8_t _load1Pin;     // LOAD pin for MAX6921 #1
    uint8_t _blank1Pin;    // BLANK pin for MAX6921 #1
    uint8_t _load2Pin;     // LOAD pin for MAX6921 #2
    uint8_t _blank2Pin;    // BLANK pin for MAX6921 #2
    
    // Display data
    uint32_t _gridData[VFD_NUM_GRIDS];    // Segment data for each grid
    uint8_t _displayBuffer[VFD_NUM_DIGITS]; // Character buffer
    uint8_t _currentGrid;                 // Current active grid
    uint8_t _brightness;                  // Display brightness (0-255)
    
    // Timing
    uint16_t _gridScanDelay;              // Grid scan delay in microseconds
    unsigned long _lastGridScan;          // Last grid scan timestamp
    
    // Internal methods
    void initializePins();
    void sendData(uint32_t data1, uint32_t data2);
    uint32_t getCharacterPattern(char character);
    
public:
    // Constructor
    MAX6921_VFD_Driver(uint8_t load1Pin = DEFAULT_MAX6921_1_LOAD_PIN,
                       uint8_t blank1Pin = DEFAULT_MAX6921_1_BLANK_PIN,
                       uint8_t load2Pin = DEFAULT_MAX6921_2_LOAD_PIN,
                       uint8_t blank2Pin = DEFAULT_MAX6921_2_BLANK_PIN);
    
    // Initialization
    bool begin();
    bool begin(uint32_t spiClockSpeed);
    
    // Basic display control
    void clear();
    void refresh();
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness();
    
    // Character and string display
    void displayCharacter(uint8_t position, char character);
    void displayString(const char* text);
    void displayString(String text);
    
    // Numeric display
    void displayNumber(int number);
    void displayNumber(long number);
    void displayFloat(float number, uint8_t decimals = 2);
    
    // Special characters and symbols
    void setDecimalPoint(uint8_t position, bool state);
    void setColon(bool state);
    void displayTime(uint8_t hours, uint8_t minutes);
    
    // Raw segment control
    void setSegment(uint8_t grid, uint8_t segment, bool state);
    void setGrid(uint8_t grid, uint32_t segmentMask);
    
    // Test and diagnostic functions
    void displayTest();
    void segmentTest();
    void gridTest();
    
    // Configuration
    void setGridScanDelay(uint16_t delayMicros);
    uint16_t getGridScanDelay();
    
    // Animation and effects
    void scrollText(const char* text, uint16_t delayMs = 200);
    void fadeIn(uint16_t durationMs = 1000);
    void fadeOut(uint16_t durationMs = 1000);
    
    // Utility functions
    bool isValidPosition(uint8_t position);
    const char* getVersion();
};

#endif // MAX6921_VFD_DRIVER_H
