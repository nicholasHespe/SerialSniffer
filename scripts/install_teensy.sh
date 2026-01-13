#!/bin/bash
#
# SerialSniffer Teensy 4.1 Development Environment Setup
# Installs Arduino IDE, Teensyduino, and required libraries
#

set -e  # Exit on error

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

echo "=========================================="
echo "SerialSniffer Teensy/Arduino Setup"
echo "=========================================="
echo ""

# Detect OS
OS="unknown"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    OS="windows"
else
    OS=$(uname -s)
fi

echo "Detected OS: $(uname -s)"
echo ""

# Installation instructions
echo "=========================================="
echo "Teensy 4.1 / Arduino IDE Setup"
echo "=========================================="
echo ""
echo "This script will guide you through the installation process."
echo "Some steps require manual installation."
echo ""

# Arduino IDE / PlatformIO check
echo "Checking for Arduino IDE or PlatformIO..."
echo ""

ARDUINO_FOUND=false
PLATFORMIO_FOUND=false

if command -v arduino-cli &> /dev/null; then
    echo "✓ Arduino CLI found"
    arduino-cli version
    ARDUINO_INSTALLED=true
else
    echo "✗ Arduino CLI not found"
    ARDUINO_INSTALLED=false
fi

if command -v pio &> /dev/null; then
    echo "✓ PlatformIO found"
    pio --version
    PLATFORMIO_INSTALLED=true
else
    echo "✗ PlatformIO not found"
    PLATFORMIO_INSTALLED=false
fi

echo ""
echo "=========================================="
echo "Teensy 4.1 Development Environment Setup"
echo "=========================================="
echo ""
echo "To develop SerialSniffer firmware, you need:"
echo ""
echo "1. Arduino IDE 2.0+ or PlatformIO"
echo "2. Teensyduino add-on"
echo "3. Teensy 4.1 board support"
echo ""
echo "Installation options:"
echo ""
echo "Option A: Arduino IDE (Recommended for beginners)"
echo "  1. Download Arduino IDE 2.0+ from:"
echo "     https://www.arduino.cc/en/software"
echo "  2. Install Teensyduino:"
echo "     https://www.pjrc.com/teensy/td_download.html"
echo "  3. Select Tools > Board > Teensy 4.1"
echo ""
echo "Option B: PlatformIO (Recommended for advanced users)"
echo "  1. Install PlatformIO Core:"
echo "     pip install platformio"
echo "  2. Configure project with platformio.ini"
echo ""
echo "Required Libraries (install via Arduino Library Manager or PlatformIO):"
echo "  - SD (built-in)"
echo "  - SPI (built-in)"
echo "  - Teensyduino core libraries"
echo ""
echo "For detailed Teensy setup instructions, see:"
echo "  https://www.pjrc.com/teensy/td_download.html"
echo ""
