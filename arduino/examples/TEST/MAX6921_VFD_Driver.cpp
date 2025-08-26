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
MAX6921_VFD_Driver::MAX6921_VFD_Driver(uint8_t loadPin, uint8_t blankPin, 
                                       uint8_t numGrids, uint8_t numSegments, uint8_t maxBrightness) {
    _loadPin = loadPin;
    _blankPin = blankPin;
    _numGrids = numGrids;
    _numSegments = numSegments;
    _maxBrightness = maxBrightness;
    
    _currentGrid = 0;
    _brightness = maxBrightness;
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
    digitalWrite(_blankPin, LOW);     // Display enabled (BLANK is active high, so LOW = display on)
}

// Send data to MAX6921 chips
void MAX6921_VFD_Driver::sendData(uint32_t data1, uint32_t data2) {
    sendDataWithMask(data1, data2);
}

// 자동 마스킹을 적용하여 데이터 전송
void MAX6921_VFD_Driver::sendDataWithMask(uint32_t data1, uint32_t data2) {
    // 사용하지 않는 비트를 0으로 마스킹
    uint32_t maskedData1 = applyChip1Mask(data1);
    uint32_t maskedData2 = applyChip2Mask(data2);
    
    // Begin SPI transaction
    SPI.beginTransaction(SPISettings(DEFAULT_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    
    // Set LOAD pin low (common for all chips)
    digitalWrite(_loadPin, LOW);
    
    // Send 20-bit data to each chip (3 bytes)
    // MAX6921 #1
    SPI.transfer((maskedData1 >> 16) & 0x0F);  // Upper 4 bits
    SPI.transfer((maskedData1 >> 8) & 0xFF);   // Middle 8 bits
    SPI.transfer(maskedData1 & 0xFF);          // Lower 8 bits
    
    // MAX6921 #2
    SPI.transfer((maskedData2 >> 16) & 0x0F);  // Upper 4 bits
    SPI.transfer((maskedData2 >> 8) & 0xFF);   // Middle 8 bits
    SPI.transfer(maskedData2 & 0xFF);          // Lower 8 bits
    
    // Set LOAD pin high to latch data (common for all chips)
    digitalWrite(_loadPin, HIGH);
    
    // End SPI transaction
    SPI.endTransaction();
}

// Clear display
void MAX6921_VFD_Driver::clear() {
    for (int i = 0; i < _numGrids; i++) {
        _gridData[i] = 0;
    }
    for (int i = 0; i < _numGrids; i++) {
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
        uint64_t segmentData = _gridData[_currentGrid];
        
        // Generic data transmission - specific bit mapping should be handled 
        // by VFD config or external mapping functions
        // For now, send raw data (this needs to be customized per VFD)
        uint32_t data1 = gridPattern | (uint32_t)(segmentData & 0xFFFFF);  // Grid + lower 20 bits
        uint32_t data2 = (uint32_t)((segmentData >> 20) & 0xFFFFF);       // Upper bits
        
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
    // BlinkTest에서는 문자 표시를 사용하지 않으므로 더미 반환
    return 0x000000;
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
    if (len > VFD_NUM_GRIDS) len = VFD_NUM_GRIDS;
    
    for (int i = 0; i < len; i++) {
        displayCharacter(i, text[i]);
    }
}

void MAX6921_VFD_Driver::displayString(String text) {
    displayString(text.c_str());
}

// Display number
void MAX6921_VFD_Driver::displayNumber(int number) {
    char buffer[VFD_NUM_GRIDS + 1];
    snprintf(buffer, sizeof(buffer), "%4d", number);
    displayString(buffer);
}

void MAX6921_VFD_Driver::displayNumber(long number) {
    char buffer[VFD_NUM_GRIDS + 1];
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
    return position < VFD_NUM_GRIDS;
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

// Set segment data for specific grid
void MAX6921_VFD_Driver::setGrid(uint8_t grid, uint64_t segmentMask) {
    if (grid < _numGrids) {
        _gridData[grid] = segmentMask;
    }
}

// Set individual segment state
void MAX6921_VFD_Driver::setSegment(uint8_t grid, uint8_t segment, bool state) {
    if (grid < _numGrids && segment < _numSegments) {
        if (state) {
            _gridData[grid] |= (1ULL << segment);  // ULL for 64-bit
        } else {
            _gridData[grid] &= ~(1ULL << segment); // ULL for 64-bit
        }
    }
}

// TODO: Implement remaining methods
// - setDecimalPoint()
// - setColon()
// - displayTime()
// - segmentTest()
// - gridTest()
// - scrollText()
// - fadeIn()
// - fadeOut()

// 직접 데이터 전송 함수 (공개 인터페이스)
void MAX6921_VFD_Driver::sendDataDirect(uint32_t data1, uint32_t data2) {
    sendData(data1, data2);
}

// 마스킹 함수들
uint32_t MAX6921_VFD_Driver::applyChip1Mask(uint32_t data) {
    return data & VFD_CHIP1_VALID_MASK;  // 첫 번째 칩: 20비트 모두 사용
}

uint32_t MAX6921_VFD_Driver::applyChip2Mask(uint32_t data) {
    return data & VFD_CHIP2_VALID_MASK;  // 두 번째 칩: 사용할 비트만 마스킹
}

// VFD 설정 정보 함수들 (디버깅용)
uint8_t MAX6921_VFD_Driver::getTotalBits() {
    return VFD_TOTAL_BITS;
}

uint8_t MAX6921_VFD_Driver::getRequiredChips() {
    return VFD_REQUIRED_CHIPS;
}

uint8_t MAX6921_VFD_Driver::getUnusedBits() {
    return VFD_UNUSED_BITS;
}

void MAX6921_VFD_Driver::printVFDInfo() {
    Serial.println("=== VFD 설정 정보 ===");
    Serial.print("VFD 그리드 수: ");
    Serial.println(VFD_NUM_GRIDS);
    Serial.print("VFD 세그먼트 수: ");
    Serial.println(VFD_NUM_SEGMENTS);
    Serial.print("총 필요 비트: ");
    Serial.println(VFD_TOTAL_BITS);
    Serial.print("필요한 MAX6921 칩 수: ");
    Serial.println(VFD_REQUIRED_CHIPS);
    Serial.print("총 출력 비트: ");
    Serial.println(VFD_TOTAL_OUTPUT_BITS);
    Serial.print("사용하지 않는 비트: ");
    Serial.println(VFD_UNUSED_BITS);
    Serial.print("첫 번째 칩 마스크: 0x");
    Serial.println(VFD_CHIP1_VALID_MASK, HEX);
    Serial.print("두 번째 칩 마스크: 0x");
    Serial.println(VFD_CHIP2_VALID_MASK, HEX);
    Serial.println("=====================");
}
