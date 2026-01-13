# Examples Directory

This directory contains example captures and analyses demonstrating SerialSniffer capabilities.

## Structure

```
examples/
├── captures/     # Example serial capture files
└── analyses/     # Example analysis outputs
```

## Captures

Place your captured serial data files here. Files are typically in CSV format with columns:
- Timestamp
- Direction (TX/RX)
- Value_Hex
- Value_ASCII
- Status

Example capture files will be added as the project develops.

## Analyses

Place your analysis outputs here (reports, visualizations, protocol documentation).

Supported output formats:
- CSV (processed data)
- XLSX (Excel reports)
- PDF (documentation)
- PNG/SVG (visualizations)

## Usage

### Capturing Data

1. Connect Teensy 4.1 to target devices
2. Start capture using SerialSniffer firmware
3. Copy capture files from SD card to `captures/`

### Analyzing Data

```bash
# Basic analysis
serialsniffer analyze captures/capture_0.csv

# Generate statistics
serialsniffer stats captures/capture_0.csv

# Detect checksums
serialsniffer checksum captures/capture_0.csv

# Visualize data
serialsniffer visualize captures/capture_0.csv -o analyses/plot.png
```

## Contributing Examples

When contributing example captures:
1. Remove sensitive data
2. Document the protocol/device
3. Include analysis results if available
4. Add description to this README
