/*
 * VFD_7BT317NK_Font.cpp
 * 
 * Implementation file for 7BT317NK VFD font functions
 * 
 * Author: Generated from VFD Config Files
 * Date: August 2025
 * Version: 1.0
 */

#include "VFD_7BT317NK_Font.h"

/**
 * Find the pattern for a given character
 * @param ch Character to look up
 * @return 21-bit pattern, or 0 if character not found
 */
uint32_t getCharacterPattern(char ch) {
    // Convert to uppercase for consistency
    if (ch >= 'a' && ch <= 'z') {
        ch = ch - 'a' + 'A';
    }
    
    // Search in font table
    for (uint8_t i = 0; i < VFD_7BT317NK_FONT_SIZE; i++) {
        if (VFD_7BT317NK_FONT_TABLE[i].character == ch) {
            return VFD_7BT317NK_FONT_TABLE[i].pattern;
        }
    }
    
    // Return space pattern if character not found
    return 0x000000;
}

/**
 * Check if character is supported in font table
 * @param ch Character to check
 * @return true if supported, false otherwise
 */
bool isCharacterSupported(char ch) {
    // Convert to uppercase for consistency
    if (ch >= 'a' && ch <= 'z') {
        ch = ch - 'a' + 'A';
    }
    
    // Search in font table
    for (uint8_t i = 0; i < VFD_7BT317NK_FONT_SIZE; i++) {
        if (VFD_7BT317NK_FONT_TABLE[i].character == ch) {
            return true;
        }
    }
    
    return false;
}

/**
 * Print font table to serial for debugging
 */
void printFontTable() {
    Serial.println("=== 7BT317NK VFD Font Table ===");
    Serial.println("Char | Pattern (21-bit)     | Hex     ");
    Serial.println("-----|---------------------|----------");
    
    for (uint8_t i = 0; i < VFD_7BT317NK_FONT_SIZE; i++) {
        char ch = VFD_7BT317NK_FONT_TABLE[i].character;
        uint32_t pattern = VFD_7BT317NK_FONT_TABLE[i].pattern;
        
        // Print character (handle special characters)
        Serial.print(" '");
        if (ch == ' ') {
            Serial.print("SP");
        } else {
            Serial.print(ch);
        }
        Serial.print("' | ");
        
        // Print binary pattern
        for (int bit = 20; bit >= 0; bit--) {
            Serial.print((pattern & (1UL << bit)) ? '1' : '0');
        }
        Serial.print(" | ");
        
        // Print hex value
        Serial.print("0x");
        if (pattern < 0x10000) Serial.print("0");
        if (pattern < 0x1000) Serial.print("0");
        if (pattern < 0x100) Serial.print("0");
        if (pattern < 0x10) Serial.print("0");
        Serial.print(pattern, HEX);
        
        Serial.println();
    }
    
    Serial.println("===============================");
    Serial.print("Total characters: ");
    Serial.println(VFD_7BT317NK_FONT_SIZE);
}

/**
 * Get segment names for debugging
 * @param pattern 21-bit segment pattern
 * @return String containing active segment names
 */
String getActiveSegments(uint32_t pattern) {
    String result = "";
    bool first = true;
    
    for (uint8_t seg = 0; seg <= 20; seg++) {
        if (pattern & (1UL << seg)) {
            if (!first) {
                result += ",";
            }
            result += "P";
            result += String(seg);
            first = false;
        }
    }
    
    if (result.length() == 0) {
        result = "None";
    }
    
    return result;
}

/**
 * Print character pattern in human-readable format
 * @param ch Character to display
 */
void printCharacterPattern(char ch) {
    uint32_t pattern = getCharacterPattern(ch);
    
    Serial.print("Character '");
    Serial.print(ch);
    Serial.print("' -> Pattern: 0x");
    Serial.print(pattern, HEX);
    Serial.print(" (");
    
    // Print binary
    for (int bit = 20; bit >= 0; bit--) {
        Serial.print((pattern & (1UL << bit)) ? '1' : '0');
    }
    
    Serial.print(") -> Active segments: ");
    Serial.println(getActiveSegments(pattern));
}
