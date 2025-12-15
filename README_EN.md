# MAX6921 VFD Arduino Driver Board

English | [한국어](README.md)

## Overview
A universal PCB circuit project for driving VFD (Vacuum Fluorescent Display) using Arduino.

## Goals
- VFD driving circuit design using max6921 driver chip
- Universal PCB board compatible with Arduino
- Future support for other VFD driver chips

## Key Features
- Based on max6921 VFD driver chip
- Arduino compatible interface
- Universal VFD driving solution
- **Multiple MAX6921 chips support (shared control pins)**
- **Enhanced versatility by removing separate power driver board**

## Power Driver

VFD power consists of two parts. One is low voltage (2~3V) for filament driving, which must be driven by AC to achieve uniform brightness across the screen. The other is high voltage required for grid and segments.

### Filament Low-voltage AC
The filament low-voltage AC operates through LM1117 - NE555 - MX612 in sequence.
- **LM1117-ADJ**: Generates voltage suitable for the filament
- **NE555**: Generates PWM signal with 50% duty cycle
- **MX612**: Switches using motor driver to create AC voltage

If desired, users can remove the 555 oscillator circuit and directly input PWM signal from MCU.

### High-voltage DC
High-voltage DC uses **MC34063**.

### Voltage Adjustment
LM1117-ADJ and MC34063 can adjust voltage using **FV-R** and **HV-R** in the schematic, respectively.

The formulas are as follows:
- Filament voltage: $FV = 1.25(1+\frac{R5}{FVR})$
- High voltage: $HV = 1.25(1+\frac{HVR}{R1})$

## VFD Examples

<details>
<summary>VFD #1 (7BT317NK)</summary>

![Grid/Segment](pics/VFD/7bt317b.jpg)
![VFD](pics/VFD/7bt317a.jpg)

Connection table
![connection table](pics/connection_table_7BT317NK.jpg)

</details>

## License
MIT License
