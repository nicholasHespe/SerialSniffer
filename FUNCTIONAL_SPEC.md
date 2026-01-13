# SerialSniffer - Functional Specifications Document

## Document Information

- **Project Name:** SerialSniffer
- **Document Version:** 1.2 (Draft)
- **Last Updated:** 2026-01-13
- **Status:** Draft

---

## 1. Executive Summary

SerialSniffer is a Teensy 4.1-based tool designed to capture, analyze, and decode serial communication between devices. It serves as a reverse engineering and debugging tool for understanding proprietary serial protocols and analyzing device communication patterns. The system includes both embedded capture firmware and a Python-based analysis suite for post-processing captured data.

### 1.1 Purpose

This document defines the functional and technical requirements for the SerialSniffer project, providing a comprehensive specification for implementation and testing.

### 1.2 Target Audience

- Hardware security researchers
- Embedded systems developers
- Protocol reverse engineers
- Electronics hobbyists and students

---

## 2. Project Scope

### 2.1 In Scope

- Real-time serial data capture and monitoring
- Multiple data format display (Hex, ASCII)
- Automatic baud rate detection
- Checksum detection and validation
- Packet analysis and boundary identification
- Data logging to SD card
- Python-based post-capture analysis suite
- Support for Teensy 4.1 platform

### 2.2 Out of Scope (Initial Release)

- Support for other microcontroller platforms
- Real-time protocol emulation/injection
- GUI-based configuration tools
- Cloud-based data storage
- Wireless protocol monitoring

---

## 3. System Overview

### 3.1 High-Level Architecture

SerialSniffer operates as a pass-through monitoring device that captures serial data transmitted between two devices without interrupting communication. The system consists of embedded capture hardware and offline analysis software.

**Components:**

1. Teensy 4.1 microcontroller (capture engine)
2. Serial interface hardware for target device monitoring
3. SD card for data logging
4. USB connection to host computer for configuration and monitoring
5. Python-based analysis suite (SerialSnifferAnalysis.py) for post-processing

### 3.2 Operating Modes

- **Monitor Mode:** Passive capture without interference
- **Debug Mode:** Detailed timing and signal quality analysis
- **Log Mode:** Continuous data capture to storage

---

## 4. Functional Requirements

### 4.1 Serial Communication Capture

#### FR-001: Baud Rate Detection

- **Priority:** High
- **Description:** System shall automatically detect baud rates of target serial communication
- **Acceptance Criteria:**
  - Automatic detection of standard baud rates: 9600, 19200, 38400, 57600, 115200
  - Detection accuracy > 95% for standard rates
  - Detection time < 2 seconds with active communication
  - Manual override capability for non-standard baud rates
  - Display detected baud rate to user 

#### FR-002: Data Format Configuration

- **Priority:** High
- **Description:** System shall support standard serial data formats
- **Acceptance Criteria:**
  - Configurable data bits (7, 8)
  - Configurable parity (None, Even, Odd)
  - Configurable stop bits (1, 2)

#### FR-003: Real-Time Data Capture

- **Priority:** High
- **Description:** System shall capture serial data in real-time
- **Acceptance Criteria:**
  - Maximum latency < 10ms for data forwarding
  - Buffer size configurable up to 1KB
  - No data loss during normal operation

#### FR-004: Checksum Detection

- **Priority:** High
- **Description:** System shall detect and validate checksums in captured data
- **Acceptance Criteria:**
  - Support common checksum algorithms (CRC8, CRC16, XOR, sum-based)
  - Automatic checksum pattern recognition
  - Validation of detected checksums against packet data
  - Report checksum errors and validation failures
  - Display checksum type and location within packets

### 4.2 Data Display and Analysis

#### FR-005: Multiple Output Formats

- **Priority:** High
- **Description:** System shall display captured data in multiple formats
- **Acceptance Criteria:**
  - Hexadecimal format with byte spacing
  - ASCII format with printable character display
  - Timestamp for each packet/byte

#### FR-006: Packet Analysis

- **Priority:** High
- **Description:** System shall perform intelligent packet analysis on captured data
- **Acceptance Criteria:**
  - Automatic packet boundary detection
  - Identify packet structure (header, payload, checksum, footer)
  - Detect repeated sequences and patterns
  - Calculate statistics (packet frequency, size distribution)
  - Export packet structure analysis
  - Support for Python-based post-processing analysis

#### FR-007: Data Filtering

- **Priority:** Medium
- **Description:** System shall provide data filtering capabilities
- **Acceptance Criteria:**
  - Filter by byte value or pattern
  - Show/hide control characters
  - Highlight specific byte sequences
  - Trigger capture on packet headers

### 4.3 Data Logging

#### FR-008: Data Export

- **Priority:** High
- **Description:** System shall log captured data to SD card for later analysis
- **Acceptance Criteria:**
  - Save data to SD card via SPI interface
  - Export to CSV format with timestamps
  - Export to hex dump format
  - Unique filename generation for each session
  - File size management and rotation capabilities

#### FR-009: Session Management

- **Priority:** Medium
- **Description:** System shall manage capture sessions
- **Acceptance Criteria:**
  - Start/stop/new capture commands
  - Clear buffer functionality
  - Session metadata (start time, duration, byte count)
  - Multiple file support for extended captures

### 4.4 Configuration and Control

#### FR-010: Runtime Configuration

- **Priority:** Medium
- **Description:** System shall allow runtime configuration changes
- **Acceptance Criteria:**
  - Change parameters via serial commands
  - Save configuration to EEPROM
  - Load configuration on startup
  - Simplified setup with automatic detection reducing manual configuration needs

#### FR-011: Status Reporting

- **Priority:** Medium
- **Description:** System shall report operational status
- **Acceptance Criteria:**
  - Buffer utilization percentage
  - Error counters (overflow, framing errors)
  - Connection status indicators
  - Detected baud rate display
  - SD card status and available space

### 4.5 Python Analysis Suite

#### FR-012: Post-Capture Analysis

- **Priority:** High
- **Description:** Python-based analysis tool (SerialSnifferAnalysis.py) shall process captured data files
- **Acceptance Criteria:**
  - Import data from SD card exports
  - Advanced pattern recognition and protocol analysis
  - Statistical analysis and visualization
  - Protocol structure documentation generation
  - Export analyzed results in multiple formats
  - Command-line interface for batch processing

---

## 5. Non-Functional Requirements

### 5.1 Performance

#### NFR-001: Throughput

- **Description:** System shall handle serial data rates up to 115200 baud without data loss
- **Measurement:** < 0.1% packet loss under sustained load

#### NFR-002: Latency

- **Description:** Pass-through latency shall be minimal
- **Measurement:** < 10ms delay for data forwarding

### 5.2 Reliability

#### NFR-003: Continuous Operation

- **Description:** System shall operate continuously without reset
- **Measurement:** 24-hour continuous operation without failure

#### NFR-004: Error Handling

- **Description:** System shall handle and report errors gracefully
- **Measurement:** All errors logged; no silent failures

### 5.3 Usability

#### NFR-005: Ease of Setup

- **Description:** Initial setup shall be straightforward
- **Measurement:** < 10 minutes from unbox to first capture

#### NFR-006: Documentation

- **Description:** Comprehensive documentation shall be provided
- **Measurement:** All features documented with examples

### 5.4 Compatibility

#### NFR-007: Hardware Platform Support

- **Description:** System shall be optimized for Teensy 4.1 platform
- **Supported Board:**
  - Teensy 4.1 (primary platform)
- **Hardware Requirements:**
  - Multiple hardware serial ports
  - SD card interface (SPI)
  - Sufficient memory for data buffering (> 512KB RAM)
  - High-speed USB for configuration and monitoring

#### NFR-008: Host Software Compatibility

- **Description:** System shall work with standard serial terminal and analysis software
- **Supported Software:**
  - Arduino Serial Monitor
  - PuTTY
  - Screen (Linux/Mac)
  - CoolTerm
  - Python 3.8+ (for SerialSnifferAnalysis.py)

---

## 6. User Interface Specifications

### 6.1 Serial Command Interface

The system shall accept commands via serial interface:

| Command  | Parameters               | Description                 |
| -------- | ------------------------ | --------------------------- |
| `START`  | Name(Optional)           | Begin data capture          |
| `STOP`   | None                     | Stop data capture           |
| `CLEAR`  | None                     | Clear capture buffer        |
| `CONFIG` | baud, data, parity, stop | Configure serial parameters |
| `MODE`   | hex/ascii/binary         | Set display mode            |
| `FILTER` | pattern                  | Set byte filter             |
| `STATUS` | None                     | Display system status       |
| `EXPORT` | format                   | Export captured data        |

### 6.2 Display Format

#### Hex Mode Output

```
[00:00:12.345] TX: 0x41 0x42 0x43 0x0D 0x0A
[00:00:12.456] RX: 0x4F 0x4B 0x0D 0x0A
```

#### ASCII Mode Output

```
[00:00:12.345] TX: ABC\r\n
[00:00:12.456] RX: OK\r\n
```

### 6.3 Status Display

```
=== SerialSniffer Status ===
Baud Rate: 9600 (Auto-detected)
Data Format: 8N1
Capture: Active
Buffer: 45% (461/1024 bytes)
Packets Captured: 127
Checksum: CRC16 detected
Errors: 0
SD Card: OK (14.2 GB free)
Uptime: 00:15:42
```

---

## 7. Hardware Interface Specifications

### 7.1 Pin Assignments (Teensy 4.1)

| Pin                    | Function           | Description                                    |
| ---------------------- | ------------------ | ---------------------------------------------- |
| Serial1 (0/1)          | Monitor Port 1     | Primary serial interface for target monitoring |
| Serial2 (7/8)          | Monitor Port 2     | Secondary serial interface (optional)          |
| SPI (MOSI/MISO/SCK/CS) | SD Card Interface  | Data logging to microSD card                   |
| USB                    | Host Communication | Configuration and real-time monitoring         |
| Pin 13                 | Status LED         | Activity indicator (built-in LED)              |

### 7.2 Connection Topology

```
Device A  <------>  Teensy 4.1  <------>  Device B
(Target)            (Sniffer)            (Target)
                         |
                         |-- SD Card (Data Logging)
                         |
                         v
                   Host Computer
                 (Configuration/Monitor)
                         |
                         v
              SerialSnifferAnalysis.py
                (Post-Processing)
```

---

## 8. Data Specifications

### 8.1 Data Capture Format

Each captured byte shall include:

- Timestamp (milliseconds since start)
- Direction (TX/RX)
- Data value (byte)
- Status flags (error, overflow, etc.)

### 8.2 Storage Format

**CSV Export Format:**

```csv
Timestamp,Direction,Value_Hex,Value_ASCII,Status
12345,TX,0x41,A,OK
12456,RX,0x4F,O,OK
```

### 8.3 Buffer Management

- Circular buffer implementation
- Configurable size (128 to 2048 bytes)
- Overflow handling with warning
- Optional buffer wrap vs. stop on full

---

## 9. Use Cases

### 9.1 Use Case 1: Basic Protocol Capture

**Actor:** Hardware Researcher
**Goal:** Capture communication between two devices
**Preconditions:** Teensy 4.1 programmed and connected, SD card inserted
**Steps:**

1. Connect Teensy 4.1 between target devices
2. Start capture (baud rate automatically detected)
3. Operate target devices to generate traffic
4. Stop capture
5. Remove SD card and download data files
6. Analyze captured data using SerialSnifferAnalysis.py

**Success Criteria:** All data captured accurately with automatic baud rate detection

### 9.2 Use Case 2: Protocol Reverse Engineering

**Actor:** Security Researcher
**Goal:** Understand unknown protocol structure
**Preconditions:** Serial traffic available, SerialSniffer deployed
**Steps:**

1. Capture extended communication session (automatic baud detection)
2. Download data files from SD card
3. Run SerialSnifferAnalysis.py on captured data
4. Review automatic packet analysis and checksum detection results
5. Use Python tools to identify protocol structure and commands
6. Document protocol specification
7. Verify findings with additional captures

**Success Criteria:** Protocol structure identified including packet format, checksums, and command set

### 9.3 Use Case 3: Debugging Serial Issues

**Actor:** Embedded Developer
**Goal:** Diagnose communication problems
**Preconditions:** Intermittent serial failures
**Steps:**

1. Insert sniffer between devices
2. Enable error reporting
3. Monitor traffic during failures
4. Analyze timing and data integrity
5. Identify root cause

**Success Criteria:** Problem identified and resolved

---

## 10. Security and Safety Considerations

### 10.1 Ethical Use

- Tool intended for authorized testing only
- Users responsible for compliance with local laws
- Not to be used for unauthorized access
- Educational and defensive security purposes

### 10.2 Electrical Safety

- Proper voltage level matching required
- Isolation recommended for high-voltage systems
- ESD protection for sensitive circuits

### 10.3 Data Privacy

- Captured data may contain sensitive information
- Users responsible for secure data handling
- No network transmission of captured data

---

## 11. Testing Requirements

### 11.1 Functional Testing

| Test ID | Description                       | Expected Result                                 |
| ------- | --------------------------------- | ----------------------------------------------- |
| FT-001  | Automatic baud rate detection     | Correct detection of 9600-115200 baud           |
| FT-002  | Checksum detection and validation | Identify and validate common checksum types     |
| FT-003  | Packet analysis                   | Accurate packet boundary detection              |
| FT-004  | Buffer overflow handling          | Graceful handling, warning issued               |
| FT-005  | Extended capture to SD card       | No data loss over 1 hour                        |
| FT-006  | SD card file export               | Valid CSV and hex dump files generated          |
| FT-007  | Python analysis suite             | Successful import and analysis of captured data |
| FT-008  | Multi-session captures            | Multiple files with unique names                |

### 11.2 Performance Testing

- Sustained throughput at maximum baud rate (115200+)
- Buffer management under load
- Memory leak testing (24+ hour operation)
- SD card write performance
- Python analysis processing time for large datasets

### 11.3 Compatibility Testing

- Testing on Teensy 4.1 hardware
- Verification with various serial terminal software
- Python analysis suite on Windows, Linux, macOS
- Different SD card types and sizes

---

## 12. Assumptions and Dependencies

### 12.1 Assumptions

- Target devices use standard UART serial communication
- Voltage levels are compatible (3.3V or 5V logic)
- Host computer has serial terminal software
- Users have basic electronics knowledge

### 12.2 Dependencies

**Hardware:**

- Teensy 4.1 microcontroller board
- MicroSD card (minimum 1GB recommended)
- USB cable for programming and monitoring

**Software:**

- Arduino IDE 2.0+ or PlatformIO
- Teensyduino add-on for Arduino IDE
- USB drivers for Teensy 4.1
- Python 3.8+ (for SerialSnifferAnalysis.py)
- Python libraries: pandas, numpy, matplotlib (for analysis suite)
- Serial terminal application (optional, for real-time monitoring)

---

## 13. Future Enhancements

### 13.1 Phase 2 Features

- Protocol decoder plugins
- Graphical user interface (desktop application)
- Advanced filtering and search capabilities
- Statistical analysis tools
- Multi-channel capture (monitor multiple serial ports)

### 13.2 Phase 3 Features

- Automated protocol inference
- Real-time protocol emulation
- Integration with logic analyzer software
- Scriptable automation
- Network-based monitoring (remote capture)

---

## 14. Glossary

| Term          | Definition                                                                       |
| ------------- | -------------------------------------------------------------------------------- |
| Baud Rate     | Number of signal changes per second in serial communication                      |
| CRC           | Cyclic Redundancy Check - error-detecting code commonly used in digital networks |
| Checksum      | Value computed from data to detect errors in transmission                        |
| Framing Error | Invalid start/stop bit in serial data                                            |
| Hex Dump      | Hexadecimal representation of binary data                                        |
| Packet        | Unit of data transmission with defined structure (header, payload, checksum)     |
| Pass-through  | Forwarding data without modification                                             |
| Protocol      | Set of rules for data communication                                              |
| Teensy 4.1    | ARM Cortex-M7 based microcontroller development board with enhanced features     |
| UART          | Universal Asynchronous Receiver/Transmitter                                      |

---

## 15. Document Approval

| Role     | Name        | Signature | Date       |
| -------- | ----------- | --------- | ---------- |
| Author   | [Your Name] |           | 2026-01-13 |
| Reviewer |             |           |            |
| Approver |             |           |            |

---

## 16. Revision History

| Version | Date       | Author | Changes                                                                                                                                                               |
| ------- | ---------- | ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1.0     | 2026-01-13 | Claude | Initial draft for review                                                                                                                                              |
| 1.1     | 2026-01-13 | Claude | Updated to reflect Teensy 4.1 platform, added automatic baud rate detection, checksum detection, packet analysis, Python analysis suite, and SD card logging features |

---

## Appendix A: References

1. Teensy 4.1 Documentation: https://www.pjrc.com/store/teensy41.html
2. Arduino/Teensyduino IDE: https://www.pjrc.com/teensy/teensyduino.html
3. Serial Communication Standards (RS-232, UART)
4. CRC and Checksum Algorithms Reference
5. Python Data Analysis Libraries (pandas, numpy, matplotlib)

## Appendix B: Command Reference

See Section 6.1 for complete command reference.
