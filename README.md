# Universal Sensor Board — Personality Module (Full Kit)

> **Team Zagtron · Gonzaga University Capstone · Sponsored by Itron**

This repository contains the complete software kit for the **Universal Sensor Board Personality Module**.  
The kit supports **embedded firmware**, a **desktop application**, and **testing tools** for local development.

---

## Project Overview

The Universal Sensor Board (USBd) Personality Module enables Itron 500G/W/CW radios to connect to a wide variety of sensors.  
It is designed to be flexible, secure, and developer-friendly, providing both embedded and host-side support.

The full kit includes:

- **Embedded Firmware (Apollo3)**
    - Runs on the ultra-low-power Ambiq Apollo3 MCU.
    - Provides sensor abstraction, secure serial link, and power management.
    - Supports I²C and digital/analog sensors, including MikroE Click modules.

- **Desktop Application (Qt 6)**
    - Connects over USB CDC/serial to the board.
    - Provides a live dashboard for sensor data.
    - Handles configuration, logging, and firmware flashing.
<!---
- **Command-Line Tools (CLI)**
    - Lightweight utilities for data capture, configuration, and testing.
    - Automates regression tests and development workflows.
-->
- **Local Simulation & Testing**
    - Host-side simulator to generate realistic data without hardware.
    - GoogleTest/CTest harness for protocol, drivers, and data handling.

---

## Tools & Technologies

- **Languages**
    - C++17 (firmware + desktop + CLI)
    - CMake (build system)
    - Optional: 
    - Python (analysis scripts, quick plotting)

- **Frameworks & SDKs**
    - AmbiqSuite SDK
    - Qt 6 (desktop UI, serial communication)
    - GoogleTest/CTest (unit testing)

- **IDEs & Tooling**
    - CLion (recommended, cross-platform CMake IDE)
    - Qt Creator (optional for desktop app)
    - Segger tools / west (for Apollo3 flashing with Zephyr)

---

## Major Features

- **Embedded Board Support**
    - Multi-sensor input (I²C, analog, digital)
    - Secure ISSI serial link with Itron radios
    - Power-aware firmware for long life in the field

- **Desktop App**
    - Real-time visualization (numeric + oscilloscope view)
    - Configuration push/pull (sampling rates, filters, channel map)
    - Data logging (CSV, Parquet)

<!---
- **CLI Utilities**
    - Device discovery and config management
    - Streaming sensor values to file
    - Loopback and CRC test harness
-->
- **Testing & Simulation**
    - Full unit test suite with GoogleTest
    - Simulator mode for demoing without hardware
    - End-to-end local runs: firmware ↔ desktop ↔ CLI

---

## Development Environment

- **Compilers**: GCC ≥ 11, MSVC 2022, or Apple Clang ≥ 14
- **CMake**: 3.25+
- **Qt**: 6.5+ (Core, Widgets, SerialPort)
- **AmbiqSuite SDK**: Required for Embedded builds

See [SETUP.md](SETUP.md) for detailed platform setup and build instructions.

---

## Structure

- firmware/ # Apollo3 firmware (AmbiqSuite)
- desktop/ # Qt 6 desktop app
- cli/ # Command-line utilities
- tests/ # GoogleTest/CTest suites
- sim/ # Host-only simulation
- docs/ # Protocol, architecture, deliverables
- scripts/ # Build, lint, CI helpers

---

## Team & Acknowledgments

- **Sponsor**: Itron
- **Institution**: Gonzaga University SEAS CEDE
- **Team Zagtron**: Fady Youssef, Jacob Alfonso, Steve Deibert

---

## License

MIT (or project-specific as required by Itron/Gonzaga).
