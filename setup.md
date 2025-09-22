# Development Setup

This guide covers building and running the **full kit**:
- **Embedded firmware** (Ambiq Apollo3 with AmbiqSuite SDK)
- **Desktop application** (Qt 6)
- **Command-line tools** (CLI utilities)
- **Local simulation and testing**

---

## 1. Environment Overview

- **Firmware**
    - Toolchain: AmbiqSuite SDK (Apollo3)
    - Build system: Makefiles (provided by SDK examples) or Segger Embedded Studio projects
    - Flashing: Segger J-Link or Apollo3 bootloader tools

- **Desktop Application**
    - Qt 6 app (Widgets + SerialPort)
    - Built with CMake and run locally (CLion recommended)
    - Provides live sensor dashboard, logging, and config

- **CLI Tools**
    - Command-line utilities for device listing, streaming, and config management
    - Built alongside the desktop app

- **Testing/Simulation**
    - Host-side simulator for development without hardware
    - GoogleTest + CTest for protocol, transport, and data model

---

## 2. Firmware Build & Flash

- Install **AmbiqSuite SDK** (from Ambiq).
- Set environment variables to point to the SDK root, e.g.:
  ```bash
  export AMBIQ_SDK_PATH=/path/to/AmbiqSuiteSDK
  export PATH=$AMBIQ_SDK_PATH/tools/gcc/bin:$PATH
- Navigate to the firmware project (e.g., firmware/apollo3/app).
- Build with Make:
  ```bash
  make clean
  make
  ```
- Flash via
  ```bash
  make flash
  ```