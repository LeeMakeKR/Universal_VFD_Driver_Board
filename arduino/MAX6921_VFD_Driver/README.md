# MAX6921 VFD Driver Library

Arduino library for controlling VFD (Vacuum Fluorescent Display) modules using MAX6921 driver ICs.

## Features

- Easy-to-use interface for VFD control
- Support for character and numeric display
- Brightness control
- Grid scanning for multiplexed displays
- Comprehensive test functions
- Optimized SPI communication

## Supported Hardware

- **VFD Display**: 7BT317NK (4-digit VFD with 7 grids, 21 segments)
- **Driver IC**: MAX6921 (20-channel VFD driver)
- **Microcontroller**: Arduino Uno/Nano/Pro Mini or compatible

## Hardware Connections

### MAX6921 #1 (Grid + Segments P0-P12)
| MAX6921 Pin | Function | Arduino Pin | Description     |
|-------------|----------|-------------|-----------------|
| Pin 20 (DIN)| Data In  | D11 (MOSI)  | SPI Data Input  |
| Pin 21 (CLK)| Clock    | D13 (SCK)   | SPI Clock       |
| Pin 22 (LOAD)| Latch   | D10 (SS)    | Data Latch      |
| Pin 23 (BLANK)| Blank  | D9          | Display Enable  |

### MAX6921 #2 (Segments P13-P20)
| MAX6921 Pin | Function | Arduino Pin | Description     |
|-------------|----------|-------------|-----------------|
| Pin 20 (DIN)| Data In  | D11 (MOSI)  | SPI Data Input  |
| Pin 21 (CLK)| Clock    | D13 (SCK)   | SPI Clock       |
| Pin 22 (LOAD)| Latch   | D8          | Data Latch      |
| Pin 23 (BLANK)| Blank  | D7          | Display Enable  |

## Installation

1. Download or clone this library
2. **IMPORTANT**: Also install the `VFD_7BT317NK_Font` library (required dependency)
3. Copy both library folders to your Arduino libraries directory:
   - `MAX6921_VFD_Driver`
   - `VFD_7BT317NK_Font`
4. Restart Arduino IDE
5. Include the library: `#include <MAX6921_VFD_Driver.h>`

## Dependencies

This library requires the `VFD_7BT317NK_Font` library, which contains the font patterns for the 7BT317NK VFD display. The font library provides:

- 44 character patterns (0-9, A-Z, and special characters)
- Optimized 21-bit segment patterns
- Character lookup functions
- Debug utilities

The font patterns are based on the segment mapping defined in `vfd-configs/7bt317nk/` directory.

## Basic Usage

```cpp
#include <MAX6921_VFD_Driver.h>

// Create driver instance with default pins
MAX6921_VFD_Driver vfd;

void setup() {
  // Initialize the driver
  vfd.begin();
  
  // Set brightness (0-255)
  vfd.setBrightness(200);
  
  // Display text
  vfd.displayString("HELLO");
}

void loop() {
  // Must call refresh() regularly to update display
  vfd.refresh();
  
  // Your code here...
}
```

## API Reference

### Initialization
- `bool begin()` - Initialize with default SPI speed
- `bool begin(uint32_t spiClockSpeed)` - Initialize with custom SPI speed

### Display Control
- `void clear()` - Clear display
- `void refresh()` - Update display (call regularly in loop)
- `void setBrightness(uint8_t brightness)` - Set brightness (0-255)
- `uint8_t getBrightness()` - Get current brightness

### Text Display
- `void displayString(const char* text)` - Display string (max 4 chars)
- `void displayString(String text)` - Display Arduino String
- `void displayCharacter(uint8_t position, char character)` - Display single character

### Numeric Display
- `void displayNumber(int number)` - Display integer
- `void displayNumber(long number)` - Display long integer
- `void displayFloat(float number, uint8_t decimals)` - Display float

### Low-Level Control
- `void setSegment(uint8_t grid, uint8_t segment, bool state)` - Control individual segment
- `void setGrid(uint8_t grid, uint32_t segmentMask)` - Set all segments for a grid

### Test Functions
- `void displayTest()` - Basic display test
- `void segmentTest()` - Test all segments
- `void gridTest()` - Test all grids

## Examples

The library includes several example sketches:

1. **SimpleDisplay** - Basic usage example
2. **DisplayTest** - Comprehensive test suite with serial commands

## Pin Customization

You can customize the control pins in the constructor:

```cpp
// Custom pin assignments
MAX6921_VFD_Driver vfd(
  10, // MAX6921 #1 LOAD pin
  9,  // MAX6921 #1 BLANK pin
  8,  // MAX6921 #2 LOAD pin
  7   // MAX6921 #2 BLANK pin
);
```

## Notes

- Always call `refresh()` regularly in your main loop
- The library uses hardware SPI (pins 11, 13 on Uno)
- VFD requires high-voltage power supply (not included)
- Tested with 7BT317NK VFD display

## License

This library is released under the MIT License.

## Contributing

Contributions are welcome! Please submit pull requests or report issues on GitHub.

## Author

Your Name (your.email@example.com)
