# SerialSniffer Project Structure

This document describes the organization of the SerialSniffer project.

## Directory Tree

```
SerialSniffer/
├── README.md                          # Project overview and quick start
├── FUNCTIONAL_SPEC.md                 # Detailed functional specifications
├── DEPENDENCIES.md                    # Complete dependency documentation
├── PROJECT_STRUCTURE.md              # This file
├── LICENSE                            # Project license (TBD)
├── .gitignore                         # Git ignore rules
├── platformio.ini                     # PlatformIO configuration
│
├── firmware/                          # Teensy 4.1 firmware
│   ├── SerialSniffer/
│   │   └── SerialSniffer.ino         # Main Arduino sketch
│   └── libraries/                     # Custom libraries (if needed)
│
├── python/                            # Python analysis suite
│   ├── SerialSnifferAnalysis.py      # Main analysis tool
│   ├── requirements.txt               # Python dependencies
│   └── setup.py                       # Python package setup
│
├── scripts/                           # Installation and setup scripts
│   ├── setup_environment.sh          # Complete setup script
│   ├── install_python.sh             # Python environment setup
│   └── install_teensy.sh             # Teensy setup guide
│
├── docs/                              # Additional documentation
│   └── (future documentation files)
│
├── examples/                          # Example captures and analyses
│   ├── README.md                      # Examples documentation
│   ├── captures/                      # Example capture files
│   └── analyses/                      # Example analysis outputs
│
└── tests/                             # Test suites
    ├── firmware_tests/                # Firmware unit tests
    └── python_tests/                  # Python unit tests
```

## Component Descriptions

### Root Directory Files

| File | Purpose |
|------|---------|
| `README.md` | Project introduction, features, quick start guide |
| `FUNCTIONAL_SPEC.md` | Detailed functional requirements and specifications |
| `DEPENDENCIES.md` | Complete list of hardware and software dependencies |
| `PROJECT_STRUCTURE.md` | This file - project organization guide |
| `.gitignore` | Git version control ignore rules |
| `platformio.ini` | PlatformIO build configuration (alternative to Arduino IDE) |

### firmware/

Contains the Teensy 4.1 embedded firmware for serial data capture.

**SerialSniffer.ino**
- Main firmware application
- Handles serial capture, SD card logging, baud detection
- Provides USB serial command interface
- Real-time monitoring and status reporting

**libraries/**
- Custom Arduino libraries (if developed)
- Currently uses built-in libraries (SD, SPI)

### python/

Contains the Python-based analysis suite for post-processing captured data.

**SerialSnifferAnalysis.py**
- Command-line interface for data analysis
- Protocol analysis and pattern recognition
- Checksum detection and validation
- Statistical analysis and visualization
- Data format conversion

**requirements.txt**
- Lists all Python package dependencies
- Used by pip for installation

**setup.py**
- Python package configuration
- Enables installation with `pip install -e .`
- Provides `serialsniffer` command-line tool

### scripts/

Contains installation and setup automation scripts.

| Script | Purpose |
|--------|---------|
| `setup_environment.sh` | Complete project setup (recommended) |
| `install_python.sh` | Install Python dependencies and tools |
| `install_teensy.sh` | Display Teensy setup instructions |

All scripts are bash-based and work on Linux, macOS, and WSL.

### docs/

Reserved for additional documentation:
- User guides
- API documentation
- Protocol reverse engineering guides
- Hardware connection diagrams
- Troubleshooting guides

### examples/

Contains example captures and analyses for learning and testing.

**captures/**
- Example serial capture files (CSV format)
- Demonstrates various protocols
- Used for testing analysis tools

**analyses/**
- Example analysis outputs
- Reports, visualizations, protocol documentation
- Demonstrates analysis capabilities

### tests/

Contains test suites for quality assurance.

**firmware_tests/**
- Unit tests for firmware components
- Integration tests for hardware
- Test fixtures and utilities

**python_tests/**
- Unit tests for Python analysis tools
- Integration tests for full pipeline
- Test data and fixtures

## File Formats

### Capture Files (CSV)

Standard format for captured serial data:

```csv
Timestamp,Direction,Value_Hex,Value_ASCII,Status
12345,RX,0x41,A,OK
12456,TX,0x42,B,OK
```

### Configuration Files

- **platformio.ini**: PlatformIO project configuration (INI format)
- **requirements.txt**: Python dependencies (plain text, one per line)
- **setup.py**: Python package metadata (Python script)

## Development Workflow

### Firmware Development

1. Edit `firmware/SerialSniffer/SerialSniffer.ino`
2. Open in Arduino IDE or use PlatformIO
3. Select Teensy 4.1 board
4. Upload to device
5. Monitor via USB serial

### Python Development

1. Edit `python/SerialSnifferAnalysis.py`
2. Install in development mode: `pip install -e python/`
3. Test with: `serialsniffer --help`
4. Run tests: `pytest tests/python_tests/`

### Adding Features

1. Update functional specifications if needed
2. Implement in firmware and/or Python
3. Add tests
4. Update documentation
5. Create example demonstrating feature

## Build Artifacts

Generated files (not in version control):

```
# Firmware builds
firmware/SerialSniffer/*.elf
firmware/SerialSniffer/*.hex
.pio/

# Python artifacts
__pycache__/
*.pyc
venv/
*.egg-info/
dist/
build/

# IDE files
.vscode/
.idea/

# Captures (user data)
examples/captures/*.csv
```

## Getting Started

### Quick Setup

```bash
# Clone repository
git clone https://github.com/nicholasHespe/SerialSniffer.git
cd SerialSniffer

# Run setup script
chmod +x scripts/*.sh
./scripts/setup_environment.sh
```

### Manual Setup

**Firmware:**
1. Install Arduino IDE and Teensyduino
2. Open `firmware/SerialSniffer/SerialSniffer.ino`
3. Upload to Teensy 4.1

**Python:**
```bash
cd python
pip install -r requirements.txt
pip install -e .
```

## Contributing

When contributing to the project:

1. Follow existing directory structure
2. Place new files in appropriate directories
3. Update relevant documentation
4. Add tests for new features
5. Update this document if structure changes

## Version Control

### What to Commit

- Source code (firmware, Python)
- Documentation
- Configuration files
- Scripts
- Example code/data (small files only)

### What Not to Commit

- Build artifacts
- Virtual environments
- User-specific IDE settings
- Large capture files
- Temporary files
- Sensitive data

See `.gitignore` for complete list.

## Support

For questions about project structure:
- Check relevant README files in subdirectories
- Review `FUNCTIONAL_SPEC.md`
- Create an issue on GitHub

## Future Structure

Planned additions:
- `docs/api/` - API documentation
- `docs/hardware/` - Wiring diagrams and schematics
- `plugins/` - Protocol decoder plugins
- `gui/` - Graphical user interface (future)
