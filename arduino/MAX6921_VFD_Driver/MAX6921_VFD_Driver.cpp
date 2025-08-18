/*
 * MAX6921_VFD_Driver.cpp
 * 
 * Implementation file for MAX6921 VFD Driver library
 * 
 * Author: Your Name
 * Date: August 2025
 * Version: 1.0
 */

#include "MAX6921_VFD_Driver.h"

// Constructor
MAX6921_VFD_Driver::MAX6921_VFD_Driver(uint8_t loadPin, uint8_t blankPin) {
    _loadPin = loadPin;
    _blankPin = blankPin;
    
    _currentGrid = 0;
    _brightness = VFD_MAX_BRIGHTNESS;
    _gridScanDelay = DEFAULT_GRID_SCAN_DELAY_US;
    _lastGridScan = 0;
    
    // Clear display data
    clear();
}

// Initialize the driver
bool MAX6921_VFD_Driver::begin() {
    return begin(DEFAULT_SPI_CLOCK_SPEED);
}

bool MAX6921_VFD_Driver::begin(uint32_t spiClockSpeed) {
    // Initialize pins
    initializePins();
    
    // Initialize SPI
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    
    // Set SPI clock speed
    if (spiClockSpeed > 8000000) spiClockSpeed = 8000000; // Max 8MHz for MAX6921
    
    // Test communication
    clear();
    sendData(0, 0);
    
    return true;
}

// Initialize hardware pins
void MAX6921_VFD_Driver::initializePins() {
    // Configure pins as outputs
    pinMode(_loadPin, OUTPUT);
    pinMode(_blankPin, OUTPUT);
    
    // Set initial states
    digitalWrite(_loadPin, HIGH);     // LOAD inactive (active low)
    digitalWrite(_blankPin, HIGH);    // Display enabled (active low)
}

// Send data to MAX6921 chips
void MAX6921_VFD_Driver::sendData(uint32_t data1, uint32_t data2) {
    // Begin SPI transaction
    SPI.beginTransaction(SPISettings(DEFAULT_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    
    // Set LOAD pin low (common for all chips)
    digitalWrite(_loadPin, LOW);
    
    // Send 20-bit data to each chip (3 bytes)
    // MAX6921 #1
    SPI.transfer((data1 >> 16) & 0x0F);  // Upper 4 bits
    SPI.transfer((data1 >> 8) & 0xFF);   // Middle 8 bits
    SPI.transfer(data1 & 0xFF);          // Lower 8 bits
    
    // MAX6921 #2
    SPI.transfer((data2 >> 16) & 0x0F);  // Upper 4 bits
    SPI.transfer((data2 >> 8) & 0xFF);   // Middle 8 bits
    SPI.transfer(data2 & 0xFF);          // Lower 8 bits
    
    // Set LOAD pin high to latch data (common for all chips)
    digitalWrite(_loadPin, HIGH);
    
    // End SPI transaction
    SPI.endTransaction();
}

// Clear display
void MAX6921_VFD_Driver::clear() {
    for (int i = 0; i < VFD_NUM_GRIDS; i++) {
        _gridData[i] = 0;
    }
    for (int i = 0; i < VFD_NUM_DIGITS; i++) {
        _displayBuffer[i] = ' ';
    }
}

// Refresh display (call regularly in main loop)
void MAX6921_VFD_Driver::refresh() {
    unsigned long currentTime = micros();
    
    if (currentTime - _lastGridScan >= _gridScanDelay) {
        // Move to next grid
        _currentGrid = (_currentGrid + 1) % VFD_NUM_GRIDS;
        
        // Calculate data for current grid
        uint32_t gridPattern = (1UL << _currentGrid);
        uint32_t segmentData = _gridData[_currentGrid];
        
        // Split data between two MAX6921 chips
        uint32_t segments1 = segmentData & 0x1FFF;        // P0-P12 (13 bits)
        uint32_t segments2 = (segmentData >> 13) & 0xFF;  // P13-P20 (8 bits)
        
        // Combine grid and segment data
        uint32_t data1 = gridPattern | (segments1 << 7);  // Grid + segments
        uint32_t data2 = segments2;                        // Segments only
        
        // Send to MAX6921 chips
        sendData(data1, data2);
        
        _lastGridScan = currentTime;
    }
}

// Set brightness (0-255)
void MAX6921_VFD_Driver::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    
    // TODO: Implement PWM brightness control using BLANK pins
    // For now, just store the value
}

uint8_t MAX6921_VFD_Driver::getBrightness() {
    return _brightness;
}

// Get character pattern from font table
uint32_t MAX6921_VFD_Driver::getCharacterPattern(char character) {
    return getVFDCharacterPattern(character);
}

// Display character at position
void MAX6921_VFD_Driver::displayCharacter(uint8_t position, char character) {
    if (!isValidPosition(position)) return;
    
    _displayBuffer[position] = character;
    
    // TODO: Implement character-to-grid mapping
    // For now, just store in buffer
    // Need to map 4 digit positions to 7 grids
}

// Display string
void MAX6921_VFD_Driver::displayString(const char* text) {
    clear();
    
    int len = strlen(text);
    if (len > VFD_NUM_DIGITS) len = VFD_NUM_DIGITS;
    
    for (int i = 0; i < len; i++) {
        displayCharacter(i, text[i]);
    }
}

void MAX6921_VFD_Driver::displayString(String text) {
    displayString(text.c_str());
}

// Display number
void MAX6921_VFD_Driver::displayNumber(int number) {
    char buffer[VFD_NUM_DIGITS + 1];
    snprintf(buffer, sizeof(buffer), "%4d", number);
    displayString(buffer);
}

void MAX6921_VFD_Driver::displayNumber(long number) {
    char buffer[VFD_NUM_DIGITS + 1];
    snprintf(buffer, sizeof(buffer), "%4ld", number);
    displayString(buffer);
}

// Display floating point number
void MAX6921_VFD_Driver::displayFloat(float number, uint8_t decimals) {
    // TODO: Implement float display with decimal point
    int intPart = (int)number;
    displayNumber(intPart);
}

// Test functions
void MAX6921_VFD_Driver::displayTest() {
    // TODO: Implement comprehensive test
    // Turn on all segments briefly
    for (int i = 0; i < VFD_NUM_GRIDS; i++) {
        _gridData[i] = 0x1FFFFF; // All segments on
    }
    delay(1000);
    clear();
}

// Utility functions
bool MAX6921_VFD_Driver::isValidPosition(uint8_t position) {
    return position < VFD_NUM_DIGITS;
}

const char* MAX6921_VFD_Driver::getVersion() {
    return MAX6921_VFD_DRIVER_VERSION;
}

// Configuration
void MAX6921_VFD_Driver::setGridScanDelay(uint16_t delayMicros) {
    _gridScanDelay = delayMicros;
}

uint16_t MAX6921_VFD_Driver::getGridScanDelay() {
    return _gridScanDelay;
}

// TODO: Implement remaining methods
// - setDecimalPoint()
// - setColon()
// - displayTime()
// - setSegment()
// - setGrid()
// - segmentTest()
// - gridTest()
// - scrollText()
// - fadeIn()
// - fadeOut()
