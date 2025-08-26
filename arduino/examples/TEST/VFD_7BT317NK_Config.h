/*
 * VFD_7BT317NK_Config.h
 * 
 * Hardware configuration for 7BT317NK VFD display
 * Defines grid count, segment count, and pin mappings specific to this VFD
 * 
 * This file provides the hardware specifications that MAX6921_VFD_Driver needs
 * to operate with the 7BT317NK display.
 * 
 * Author: Generated from VFD Config Files
 * Date: August 2025
 * Version: 1.0
 */

#ifndef VFD_7BT317NK_CONFIG_H
#define VFD_7BT317NK_CONFIG_H

#include <Arduino.h>

// VFD Hardware Specifications
#define VFD_NUM_GRIDS      7     // G0-G6 (그리드 수 = 자릿수)
#define VFD_NUM_SEGMENTS   21    // P0-P20 (세그먼트 수)
#define VFD_MAX_BRIGHTNESS 255   // 최대 밝기 레벨

// Note: MAX6921 하드웨어 사양과 비트 계산은 MAX6921_VFD_Driver.h에서 정의됨

// Grid pin assignments for MAX6921 chips
// G0-G6 are mapped to specific output pins on the MAX6921 chips
#define VFD_GRID_G0_CHIP    1    // First MAX6921 chip
#define VFD_GRID_G0_PIN     0    // OUT0
#define VFD_GRID_G1_CHIP    1    // First MAX6921 chip  
#define VFD_GRID_G1_PIN     1    // OUT1
#define VFD_GRID_G2_CHIP    1    // First MAX6921 chip
#define VFD_GRID_G2_PIN     2    // OUT2
#define VFD_GRID_G3_CHIP    1    // First MAX6921 chip
#define VFD_GRID_G3_PIN     3    // OUT3
#define VFD_GRID_G4_CHIP    1    // First MAX6921 chip
#define VFD_GRID_G4_PIN     4    // OUT4
#define VFD_GRID_G5_CHIP    1    // First MAX6921 chip
#define VFD_GRID_G5_PIN     5    // OUT5
#define VFD_GRID_G6_CHIP    1    // First MAX6921 chip
#define VFD_GRID_G6_PIN     6    // OUT6

// Segment distribution across MAX6921 chips
// 실제 하드웨어 연결에 맞춘 정확한 매핑:
// MAX6921 #1 (20비트): OUT0-OUT6 (G0-G6) + OUT7-OUT19 (S0-S12)
// MAX6921 #2 (20비트): OUT0-OUT7 (S13-S20) + OUT8-OUT19 (사용안함)

// 첫 번째 MAX6921의 데이터 구성
#define VFD_MAX1_GRID_BITS        7      // G0-G6 (bit 0-6)
#define VFD_MAX1_SEGMENT_BITS     13     // S0-S12 (bit 7-19)
#define VFD_MAX1_GRID_MASK        0x0007F    // G0-G6 (bit 0-6)
#define VFD_MAX1_SEGMENT_MASK     0x1FFF80   // S0-S12 (bit 7-19)

// 두 번째 MAX6921의 데이터 구성  
#define VFD_MAX2_SEGMENT_BITS     8      // S13-S20 (bit 0-7)
#define VFD_MAX2_SEGMENT_MASK     0x0FF      // S13-S20 (bit 0-7)

// Helper macros for segment mapping
#define VFD_ALL_SEGMENTS_MAX1     VFD_MAX1_SEGMENT_MASK
#define VFD_ALL_SEGMENTS_MAX2     VFD_MAX2_SEGMENT_MASK

// Grid mask generation functions
inline uint32_t getGridMask(int grid) {
    if (grid < 0 || grid >= VFD_NUM_GRIDS) {
        return 0;
    }
    return (1UL << grid);  // G0=bit0, G1=bit1, ..., G6=bit6
}

// Grid validation function
inline bool isValidGrid(int grid) {
    return (grid >= 0 && grid < VFD_NUM_GRIDS);
}

// Segment validation function
inline bool isValidSegment(int segment) {
    return (segment >= 0 && segment < VFD_NUM_SEGMENTS);
}

#endif // VFD_7BT317NK_CONFIG_H
