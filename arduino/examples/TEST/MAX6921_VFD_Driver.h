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

// Note: VFD-specific configurations (VFD_NUM_GRIDS, VFD_NUM_SEGMENTS, VFD_MAX_BRIGHTNESS) 
// must be defined by including appropriate VFD config file BEFORE including this header

// MAX6921 하드웨어 사양
#define MAX6921_OUTPUT_BITS 20    // MAX6921 한 개당 출력 비트 수

// VFD 설정을 기반으로 한 자동 계산 (VFD config 파일이 먼저 include되어야 함)
#define VFD_TOTAL_BITS (VFD_NUM_GRIDS + VFD_NUM_SEGMENTS)  // 총 필요 비트
#define VFD_REQUIRED_CHIPS ((VFD_TOTAL_BITS + MAX6921_OUTPUT_BITS - 1) / MAX6921_OUTPUT_BITS)  // 올림 계산
#define VFD_TOTAL_OUTPUT_BITS (VFD_REQUIRED_CHIPS * MAX6921_OUTPUT_BITS)  // 총 출력 비트
#define VFD_UNUSED_BITS (VFD_TOTAL_OUTPUT_BITS - VFD_TOTAL_BITS)  // 사용하지 않는 비트

// 데이터 마스크 정의 (사용하지 않는 비트를 0으로 만들기 위해)
#define VFD_CHIP1_VALID_MASK ((1UL << MAX6921_OUTPUT_BITS) - 1)  // 첫 번째 칩: 20비트 모두 사용
#define VFD_CHIP2_VALID_BITS (VFD_TOTAL_BITS - MAX6921_OUTPUT_BITS)  // 두 번째 칩에서 사용할 비트 수
#define VFD_CHIP2_VALID_MASK ((1UL << VFD_CHIP2_VALID_BITS) - 1)  // 두 번째 칩: 사용할 비트만 마스킹


// Library version
#define MAX6921_VFD_DRIVER_VERSION "1.0.0"

// Note: Pin assignments must be defined in VFD config file

// Timing constants
#define DEFAULT_GRID_SCAN_DELAY_US  2000  // Microseconds per grid
#define DEFAULT_SPI_CLOCK_SPEED     4000000  // 4MHz SPI clock

class MAX6921_VFD_Driver {
private:
    // Hardware pin assignments
    uint8_t _loadPin;      // Common LOAD pin for all MAX6921 chips
    uint8_t _blankPin;     // Common BLANK pin for all MAX6921 chips
    
    // VFD configuration (set in constructor)
    uint8_t _numGrids;     // Number of grids for this VFD
    uint8_t _numSegments;  // Number of segments for this VFD
    uint8_t _maxBrightness; // Maximum brightness for this VFD
    
    // Display data (using maximum possible values to avoid dependency)
    // Each grid can have up to 64 segments (using 64-bit data type)
    uint64_t _gridData[16];    // Segment data for each grid (max 16 grids, 64 segments each)
    uint8_t _displayBuffer[16]; // Character buffer (max 16 characters)
    uint8_t _currentGrid;                 // Current active grid
    uint8_t _brightness;                  // Display brightness (0-255)
    
    // Timing
    uint16_t _gridScanDelay;              // Grid scan delay in microseconds
    unsigned long _lastGridScan;          // Last grid scan timestamp
    
    // Internal methods
    void initializePins();
    void sendData(uint32_t data1, uint32_t data2);
    void sendDataWithMask(uint32_t data1, uint32_t data2);  // 자동 마스킹 포함
    uint32_t getCharacterPattern(char character);
    
    // 자동 마스킹 함수들
    uint32_t applyChip1Mask(uint32_t data);
    uint32_t applyChip2Mask(uint32_t data);
    
public:
    // Constructor - pin assignments and VFD configuration must be provided
    MAX6921_VFD_Driver(uint8_t loadPin, uint8_t blankPin, 
                       uint8_t numGrids, uint8_t numSegments, uint8_t maxBrightness);
    
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
    void setGrid(uint8_t grid, uint64_t segmentMask);
    void sendDataDirect(uint32_t data1, uint32_t data2);  // 직접 데이터 전송
    
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
    
    // VFD 설정 정보 함수들 (디버깅용)
    uint8_t getTotalBits();
    uint8_t getRequiredChips(); 
    uint8_t getUnusedBits();
    void printVFDInfo();  // 시리얼로 VFD 설정 정보 출력
    const char* getVersion();
};

#endif // MAX6921_VFD_DRIVER_H
