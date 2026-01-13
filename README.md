# SerialSniffer

An Arduino-based tool for reverse engineering and decoding serial communication protocols.

## Overview

SerialSniffer is designed to capture, analyze, and decode serial communication between devices. This is useful for:
- Reverse engineering proprietary serial protocols
- Debugging serial communication issues
- Understanding device communication patterns
- Analyzing data transmission formats

## Hardware Requirements

- Supported Boards: Teensy 4.1
- Serial communication interface
- Target devices for monitoring

## Features

- Real-time serial data capture
- Hex and ASCII output modes
- Baud Rate Detection
- Check Sum Detection
- Packet Analysis
- Data logging capabilities

## Getting Started

1. Connect your Arduino to your computer
2. Upload the SerialSniffer sketch
3. Monitor and analyze captured data
4. Download Output files to be analysed with the python program

## Project Structure

```
SerialSniffer/
├── README.md
├── SerialSniffer.ino    # Main Arduino sketch
├── SerialSnifferAnalysis.py
└── examples/            # Example captures and analyses
```

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

TBD
