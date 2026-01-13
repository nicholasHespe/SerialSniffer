# SerialSniffer

A Teensy 4.1-based tool for reverse engineering and decoding serial communication protocols.

## Overview

SerialSniffer is designed to capture, analyze, and decode serial communication between devices. It combines hardware-based capture with powerful Python-based analysis tools.

**Use Cases:**
- Reverse engineering proprietary serial protocols
- Debugging serial communication issues
- Understanding device communication patterns
- Analyzing data transmission formats
- Protocol documentation and specification

## Features

### Hardware Capture (Teensy 4.1)
- ⚡ Real-time serial data capture
- 🔍 Automatic baud rate detection (9600-115200)
- ✅ Checksum detection and validation (CRC8, CRC16, XOR, Sum)
- 📦 Intelligent packet analysis
- 💾 SD card data logging
- 🖥️ USB serial monitoring and configuration

### Python Analysis Suite
- 📊 Statistical analysis and visualization
- 🔎 Advanced pattern recognition
- 📝 Protocol structure documentation
- 🔄 Multiple export formats (CSV, Excel, JSON)
- 🖼️ Data visualization and plotting
- 🧮 Custom checksum validation

## Hardware Requirements

- **Teensy 4.1** microcontroller board
- **MicroSD card** (1GB+ recommended)
- **USB cable** (Micro-USB)
- **Target serial devices** to monitor (3.3V or 5V logic)

## Software Requirements

- **Firmware Development:**
  - Arduino IDE 2.0+ with Teensyduino add-on, OR
  - PlatformIO

- **Analysis Suite:**
  - Python 3.8 or higher
  - See `python/requirements.txt` for Python packages

## Quick Start

### Automated Setup (Recommended)

```bash
# Clone the repository
git clone https://github.com/nicholasHespe/SerialSniffer.git
cd SerialSniffer

# Run the setup script
chmod +x scripts/*.sh
./scripts/setup_environment.sh
```

### Manual Setup

#### 1. Firmware Setup

**Option A: Arduino IDE**
1. Install [Arduino IDE 2.0+](https://www.arduino.cc/en/software)
2. Install [Teensyduino](https://www.pjrc.com/teensy/td_download.html)
3. Open `firmware/SerialSniffer/SerialSniffer.ino`
4. Select **Tools > Board > Teensy 4.1**
5. Upload to your Teensy 4.1

**Option B: PlatformIO**
```bash
# Install PlatformIO
pip install platformio

# Build and upload
pio run --target upload
```

#### 2. Python Setup

```bash
# Navigate to Python directory
cd python

# Install dependencies
pip install -r requirements.txt

# Install in development mode
pip install -e .
```

### Using SerialSniffer

#### 1. Capture Data

1. Insert MicroSD card into Teensy 4.1
2. Connect Teensy between target serial devices
3. Connect Teensy to computer via USB
4. Open serial terminal (115200 baud)
5. Send `s` command to start capture
6. Automatic baud rate detection will occur
7. Data is logged to SD card in real-time
8. Send `t` command to stop capture

#### 2. Analyze Captured Data

```bash
# View statistics
serialsniffer stats capture_0.csv

# Analyze protocol structure
serialsniffer analyze capture_0.csv

# Detect and validate checksums
serialsniffer checksum capture_0.csv

# Visualize data patterns
serialsniffer visualize capture_0.csv -o plot.png

# Convert to different format
serialsniffer convert capture_0.csv -o output.xlsx
```

## Project Structure

```
SerialSniffer/
├── README.md                      # This file
├── FUNCTIONAL_SPEC.md            # Detailed specifications
├── DEPENDENCIES.md               # Dependency documentation
├── PROJECT_STRUCTURE.md          # Project organization guide
│
├── firmware/                      # Teensy 4.1 firmware
│   └── SerialSniffer/
│       └── SerialSniffer.ino     # Main sketch
│
├── python/                        # Python analysis suite
│   ├── SerialSnifferAnalysis.py  # Main analysis tool
│   ├── requirements.txt           # Dependencies
│   └── setup.py                   # Package setup
│
├── scripts/                       # Setup scripts
│   ├── setup_environment.sh      # Complete setup
│   ├── install_python.sh         # Python setup
│   └── install_teensy.sh         # Teensy setup guide
│
├── examples/                      # Example captures
│   ├── captures/                  # Sample data files
│   └── analyses/                  # Sample outputs
│
└── tests/                         # Test suites
    ├── firmware_tests/
    └── python_tests/
```

See `PROJECT_STRUCTURE.md` for detailed documentation.

## Documentation

- **[FUNCTIONAL_SPEC.md](FUNCTIONAL_SPEC.md)** - Complete functional requirements and specifications
- **[DEPENDENCIES.md](DEPENDENCIES.md)** - Hardware and software dependencies
- **[PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)** - Project organization and file structure
- **[examples/README.md](examples/README.md)** - Example usage and files

## Firmware Commands

Connect via serial terminal (115200 baud) and use these commands:

| Command | Description |
|---------|-------------|
| `s` | Start capture (auto-detect baud) |
| `t` | Stop capture |
| `n` | Create new capture file |
| `c` | Clear buffer |
| `i` | Show status and statistics |
| `h` | Show help menu |

## Python CLI Commands

```bash
# Show help
serialsniffer --help

# Analyze data
serialsniffer analyze <file>

# Show statistics
serialsniffer stats <file>

# Detect checksums
serialsniffer checksum <file> [--algorithm crc16]

# Visualize patterns
serialsniffer visualize <file> [--output plot.png]

# Analyze packets
serialsniffer packets <file>

# Convert formats
serialsniffer convert <file> [--format xlsx]
```

## Development

### Running Tests

```bash
# Python tests
cd python
pytest tests/

# Firmware tests
# (Open in Arduino IDE or use PlatformIO)
```

### Code Style

```bash
# Format Python code
black python/SerialSnifferAnalysis.py

# Lint Python code
flake8 python/

# Type checking
mypy python/
```

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Update documentation
6. Submit a pull request

See `PROJECT_STRUCTURE.md` for development workflow details.

## Troubleshooting

### Teensy Not Recognized
- Install Teensyduino
- Check USB cable and port
- Try pressing the program button on Teensy

### SD Card Issues
- Format as FAT32
- Try a different SD card
- Check SD card is properly inserted

### Python Installation Issues
- Ensure Python 3.8+ is installed
- Use virtual environment: `python -m venv venv`
- Activate venv before installing: `source venv/bin/activate`

### Serial Port Access (Linux)
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in
```

## Support

- **Issues:** [GitHub Issues](https://github.com/nicholasHespe/SerialSniffer/issues)
- **Documentation:** See `docs/` directory
- **Examples:** See `examples/` directory

## Roadmap

- [ ] Advanced protocol decoders
- [ ] Real-time visualization GUI
- [ ] Multi-channel capture support
- [ ] Protocol emulation/injection
- [ ] Automated protocol inference
- [ ] Cloud-based analysis

See `FUNCTIONAL_SPEC.md` for complete feature roadmap.

## License

TBD

## Acknowledgments

- Built on Teensy 4.1 by PJRC
- Uses Arduino/Teensyduino framework
- Python libraries: pandas, matplotlib, click, rich

---

**Made with ❤️ for hardware hackers and protocol reverse engineers**
