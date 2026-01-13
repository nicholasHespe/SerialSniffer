# SerialSniffer Dependencies

This document outlines all dependencies required for the SerialSniffer project.

## Hardware Dependencies

### Required Hardware

| Component | Specification | Purpose |
|-----------|--------------|---------|
| Teensy 4.1 | ARM Cortex-M7 @ 600MHz | Main microcontroller for data capture |
| MicroSD Card | 1GB+ recommended | Data logging and storage |
| USB Cable | Micro-USB | Programming and serial communication |
| Target Serial Devices | 3.3V or 5V logic | Devices to monitor |

### Optional Hardware

- Logic level shifters (if voltage levels don't match)
- External power supply (for standalone operation)
- Breadboard and jumper wires
- Status LEDs (additional indicators)

## Firmware Dependencies

### Development Environment

**Option A: Arduino IDE**
- Arduino IDE 2.0 or higher
- Download: https://www.arduino.cc/en/software

**Option B: PlatformIO**
- PlatformIO Core or PlatformIO IDE
- Installation: `pip install platformio`
- Documentation: https://platformio.org/

### Teensy Support

| Component | Version | Installation |
|-----------|---------|--------------|
| Teensyduino | Latest | https://www.pjrc.com/teensy/td_download.html |
| Teensy Loader | Latest (included) | Automatic with Teensyduino |
| USB Drivers | OS-dependent | Automatic on most systems |

### Arduino Libraries

All libraries are available through Arduino Library Manager or PlatformIO:

| Library | Version | Purpose | License |
|---------|---------|---------|---------|
| SD | Built-in | SD card access | MIT |
| SPI | Built-in | SPI communication | MIT |
| Teensyduino Core | Latest | Teensy 4.1 support | MIT |

### Build Tools

- GNU ARM Embedded Toolchain (included with Teensyduino)
- Make (optional, for command-line builds)

## Python Dependencies

### Python Version

- **Required:** Python 3.8 or higher
- **Recommended:** Python 3.10+
- Download: https://www.python.org/downloads/

### Core Dependencies

Listed in `python/requirements.txt`:

| Package | Version | Purpose |
|---------|---------|---------|
| pandas | >=2.0.0 | Data manipulation and analysis |
| numpy | >=1.24.0 | Numerical computing |
| matplotlib | >=3.7.0 | Data visualization |
| seaborn | >=0.12.0 | Statistical visualization |
| pyserial | >=3.5 | Serial communication |
| openpyxl | >=3.1.0 | Excel file export |
| crcmod | >=1.7 | CRC checksum calculations |
| click | >=8.1.0 | CLI framework |
| rich | >=13.0.0 | Terminal output formatting |

### Development Dependencies

| Package | Version | Purpose |
|---------|---------|---------|
| pytest | >=7.4.0 | Testing framework |
| pytest-cov | >=4.1.0 | Test coverage |
| black | >=23.0.0 | Code formatting |
| flake8 | >=6.0.0 | Linting |
| mypy | >=1.0.0 | Type checking |

### Installation

```bash
# Install core dependencies
pip install -r python/requirements.txt

# Install with development tools
pip install -r python/requirements.txt
pip install pytest pytest-cov black flake8 mypy

# Or install in development mode
cd python
pip install -e .
pip install -e ".[dev]"
```

## System Dependencies

### Operating System Support

| OS | Supported | Notes |
|----|-----------|-------|
| Linux | ✅ Yes | Ubuntu 20.04+, Debian, Fedora, etc. |
| macOS | ✅ Yes | macOS 10.15+ |
| Windows | ✅ Yes | Windows 10/11 |
| WSL | ✅ Yes | Windows Subsystem for Linux |

### System Tools

| Tool | Required For | Installation |
|------|-------------|--------------|
| Git | Version control | https://git-scm.com/ |
| Python 3.8+ | Analysis suite | https://www.python.org/ |
| pip | Python packages | Included with Python |
| USB drivers | Teensy connection | Automatic on most systems |

### Linux-Specific Dependencies

```bash
# Ubuntu/Debian
sudo apt-get install python3 python3-pip git libusb-dev

# Fedora
sudo dnf install python3 python3-pip git libusb-devel

# Arch Linux
sudo pacman -S python python-pip git libusb
```

### macOS-Specific Dependencies

```bash
# Using Homebrew
brew install python git libusb
```

### Windows-Specific Dependencies

- Python 3.8+ from python.org
- Git from git-scm.com
- USB drivers (automatic with Teensyduino)

## Serial Terminal Software (Optional)

For real-time monitoring, any of these tools work:

| Software | Platform | Download |
|----------|----------|----------|
| Arduino Serial Monitor | All | Built into Arduino IDE |
| PuTTY | Windows | https://www.putty.org/ |
| CoolTerm | All | https://freeware.the-meiers.org/ |
| screen | Linux/Mac | `apt install screen` or built-in |
| minicom | Linux | `apt install minicom` |

## Version Requirements Summary

### Minimum Requirements

```
Hardware:
  - Teensy 4.1
  - MicroSD card (1GB+)
  - USB cable

Software:
  - Python 3.8+
  - Arduino IDE 2.0+ or PlatformIO
  - Teensyduino (latest)
  - Git

Storage:
  - ~500MB for development environment
  - 1GB+ for SD card captures
  - ~100MB for Python virtual environment
```

### Recommended Configuration

```
Hardware:
  - Teensy 4.1
  - High-quality MicroSD card (Class 10, 8GB+)
  - Quality USB cable
  - Logic level shifters if needed

Software:
  - Python 3.10+
  - Arduino IDE 2.3+
  - Latest Teensyduino
  - Virtual environment for Python

Storage:
  - 1GB+ for development
  - 8GB+ SD card
  - SSD for better performance
```

## Installation Scripts

Automated installation scripts are provided in the `scripts/` directory:

| Script | Purpose |
|--------|---------|
| `setup_environment.sh` | Complete setup (recommended) |
| `install_python.sh` | Python dependencies only |
| `install_teensy.sh` | Teensy setup guide |

### Quick Setup

```bash
# Clone repository
git clone https://github.com/nicholasHespe/SerialSniffer.git
cd SerialSniffer

# Run complete setup
chmod +x scripts/*.sh
./scripts/setup_environment.sh
```

## Troubleshooting

### Common Issues

**Python Installation**
- Issue: "python3 not found"
- Solution: Install Python 3.8+ from python.org or your package manager

**Teensy Not Recognized**
- Issue: USB device not detected
- Solution: Install Teensyduino, check USB cable, try different port

**SD Card Problems**
- Issue: SD card not accessible
- Solution: Format as FAT32, check wiring, verify CS pin

**Permission Denied (Linux)**
- Issue: Cannot access serial port
- Solution: Add user to dialout group: `sudo usermod -a -G dialout $USER`

## License Information

### Third-Party Licenses

All dependencies maintain their respective licenses:
- Python packages: Mostly BSD/MIT
- Arduino libraries: MIT
- Teensy cores: MIT
- Teensyduino: PJRC.COM License

Refer to individual package documentation for complete license information.

## Updates and Maintenance

Dependencies should be regularly updated for security and features:

```bash
# Update Python packages
pip install --upgrade -r python/requirements.txt

# Update Arduino libraries
# Use Library Manager in Arduino IDE or:
pio lib update  # For PlatformIO users
```

## Support

For dependency-related issues:
1. Check documentation in `docs/`
2. Review `README.md`
3. Create an issue: https://github.com/nicholasHespe/SerialSniffer/issues
