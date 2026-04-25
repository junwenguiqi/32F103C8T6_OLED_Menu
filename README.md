# STM32F103C8T6 OLED Menu

This project is based on the STM32F103 series microcontroller and includes modules such as OLED menu, encoder input, TB6612 motor driver control, and MPU6050 sensor reading. It uses a Keil uVision project file and can be compiled, downloaded, and debugged directly in Keil.

## Project Overview
- MCU: STM32F103C8T6
- Display: OLED (driver and fonts in `Hardware/OLED.c` / `Hardware/OLED_Font.h`)
- Motor Driver: TB6612 (driver in `Hardware/Motor.c`)
- Sensor: MPU6050 (`Hardware/MPU6050.c` / `Hardware/MPU6050.h`)
- Input: Encoder and buttons (`Hardware/Encoder.c`, `Hardware/Key.c`)

## Directory Structure (Summary)
- `User/` - Main program and interrupt interface (`User/main.c`, `User/stm32f10x_it.c`)
- `Hardware/` - Hardware-related drivers and modules (e.g., `OLED.c`, `Motor.c`, `MyI2C.c`, `MPU6050.c`, etc.)
- `Library/` - STM32 standard peripheral library source files (`stm32f10x_*.c/.h`)
- `Start/` - Startup files and system clock configuration (`system_stm32f10x.c`, startup assembly file)
- `DebugConfig/` - Debug configuration
- `Project.uvprojx`, `Project.uvguix.*` - Keil project files

## Dependencies
- STM32F10x standard peripheral library (source code included in `Library/`)
- Keil uVision (a version that supports STM32F1 is recommended)
- Download/Debugger: ST-Link, USB-to-serial, or other compatible tools

## Quick Start
1. Open the project file `Project.uvprojx` with Keil.
2. Configure the toolchain and debugger (e.g., select ST-Link as the debugger).
3. Build the project.
4. Download to the target board and run/debug.

## Key Source Code Description
- `User/main.c`: Program entry point, initialization, and main loop.
- `Hardware/OLED.c`, `Hardware/OLED_Font.h`: OLED display driver and font table.
- `Hardware/menu.c`: Menu logic and display refresh.
- `Hardware/MyI2C.c`: I2C communication wrapper (for OLED, MPU6050, etc.).
- `Hardware/Motor.c` / `Hardware/PWM.c`: Motor and PWM control related.

## Debugging and Development Suggestions
- When adding new peripherals or making modifications, enable them step by step in `main.c` and verify each module using serial output or LEDs.
