/*
 * SerialSniffer - Serial Protocol Analysis Tool
 *
 * Hardware: Teensy 4.1
 * Features:
 *   - Automatic baud rate detection
 *   - Checksum detection and validation
 *   - Packet analysis
 *   - SD card data logging
 *   - Real-time serial monitoring
 *
 * Author: SerialSniffer Team
 * License: TBD
 */

#include <SD.h>
#include <SPI.h>
#include "SerialSniffer.h"

// ==================== Configuration ====================

// Pin definitions
const int LED_PIN = 13;           // Built-in LED for status indication
const int SD_CS_PIN = BUILTIN_SDCARD;  // Teensy 4.1 built-in SD card

// Serial port configuration
#define TARGET_SERIAL Serial1     // Hardware serial for monitoring
#define DEBUG_SERIAL Serial       // USB serial for debugging/configuration

// Buffer configuration
const int BUFFER_SIZE = 1024;
uint8_t rxBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Baud rate detection
const long baudRates[] = {9600, 19200, 38400, 57600, 115200};
const int numBaudRates = sizeof(baudRates) / sizeof(baudRates[0]);
long detectedBaud = 0;

// SD card logging
File dataFile;
String currentFilename = "";
bool sdCardReady = false;

// Statistics
unsigned long bytesReceived = 0;
unsigned long packetsDetected = 0;
unsigned long startTime = 0;

// State machine
enum CaptureState {
  IDLE,
  DETECTING_BAUD,
  AWAITING_MANUAL_BAUD,
  CAPTURING,
  STOPPED
};
CaptureState currentState = IDLE;

// Baud detection state
volatile uint32_t edgeTimes[50];  // Store edge timestamps
volatile int edgeCount = 0;       // Number of edges captured

// ==================== Setup ====================

void setup() {
  // Initialize USB Serial for debugging
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL && millis() < 3000) {
    ; // Wait for serial port or timeout
  }

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Welcome message
  DEBUG_SERIAL.println("========================================");
  DEBUG_SERIAL.println("     SerialSniffer v0.1.0");
  DEBUG_SERIAL.println("     Teensy 4.1 Serial Protocol Analyzer");
  DEBUG_SERIAL.println("========================================");
  DEBUG_SERIAL.println();

  // Initialize SD card
  DEBUG_SERIAL.print("Initializing SD card...");
  if (SD.begin(SD_CS_PIN)) {
    sdCardReady = true;
    DEBUG_SERIAL.println(" OK");
    DEBUG_SERIAL.print("SD Card Type: ");
    printSDCardInfo();
  } else {
    DEBUG_SERIAL.println(" FAILED");
    DEBUG_SERIAL.println("WARNING: SD card not available. Logging disabled.");
  }
  DEBUG_SERIAL.println();

  // Set default baud rate
  detectedBaud = 9600;
  DEBUG_SERIAL.print("Default baud rate: ");
  DEBUG_SERIAL.println(detectedBaud);
  DEBUG_SERIAL.println("Use 'd' command to auto-detect, or 'b' to set manually.");
  DEBUG_SERIAL.println();

  // Display menu
  printMenu();

  startTime = millis();
}

// ==================== Main Loop ====================

void loop() {
  // Handle debug commands
  if (DEBUG_SERIAL.available()) {
    handleCommand();
  }

  // State machine
  switch (currentState) {
    case IDLE:
      // Waiting for command
      break;

    case DETECTING_BAUD:
      // Detection runs from handleCommand, not here
      break;

    case AWAITING_MANUAL_BAUD:
      // Waiting for user input (handled in handleCommand)
      break;

    case CAPTURING:
      // Capture data from target serial port
      captureData();
      blinkLED();
      break;

    case STOPPED:
      // Capture stopped
      break;
  }

  delay(10);
}

// ==================== Functions ====================

void printMenu() {
  DEBUG_SERIAL.println("Commands:");
  DEBUG_SERIAL.println("  s - Start capture (uses current baud rate)");
  DEBUG_SERIAL.println("  t - Stop capture");
  DEBUG_SERIAL.println("  d - Detect baud rate automatically");
  DEBUG_SERIAL.println("  b - Set baud rate manually");
  DEBUG_SERIAL.println("  n - New capture file");
  DEBUG_SERIAL.println("  c - Clear buffer");
  DEBUG_SERIAL.println("  i - Show status/info");
  DEBUG_SERIAL.println("  h - Show this help menu");
  DEBUG_SERIAL.println();
}

void handleCommand() {
  char cmd = DEBUG_SERIAL.read();

  // Handle manual baud rate input if in that state
  if (currentState == AWAITING_MANUAL_BAUD) {
    handleManualBaudInput(cmd);
    return;
  }

  switch (cmd) {
    case 's':
    case 'S':
      startCapture();
      break;

    case 't':
    case 'T':
      stopCapture();
      break;

    case 'd':
    case 'D':
      DEBUG_SERIAL.println("Starting baud rate detection...");
      DEBUG_SERIAL.println("Make sure target device is transmitting data.");
      currentState = DETECTING_BAUD;
      detectBaudRate();
      break;

    case 'b':
    case 'B':
      promptManualBaudRate();
      break;

    case 'n':
    case 'N':
      newCaptureFile();
      break;

    case 'c':
    case 'C':
      clearBuffer();
      break;

    case 'i':
    case 'I':
      printStatus();
      break;

    case 'h':
    case 'H':
      printMenu();
      break;

    case '\r':
    case '\n':
      // Ignore newlines
      break;

    default:
      DEBUG_SERIAL.println("Unknown command. Press 'h' for help.");
      break;
  }
}

void startCapture() {
  DEBUG_SERIAL.println("Starting capture...");

  // Create new log file if needed
  if (currentFilename.length() == 0) {
    newCaptureFile();
  }

  // Open the capture file for writing (keep it open during capture)
  if (sdCardReady) {
    dataFile = SD.open(currentFilename.c_str(), FILE_WRITE);
    if (!dataFile) {
      DEBUG_SERIAL.println("ERROR: Could not open capture file for writing.");
      return;
    }
  }

  // Start baud rate detection
  currentState = DETECTING_BAUD;
  DEBUG_SERIAL.println("Detecting baud rate...");

  // For now, use a default baud rate
  // TODO: Implement auto-detection
  detectedBaud = 9600;
  TARGET_SERIAL.begin(detectedBaud);

  DEBUG_SERIAL.print("Using baud rate: ");
  DEBUG_SERIAL.println(detectedBaud);

  currentState = CAPTURING;
  startTime = millis();
  DEBUG_SERIAL.println("Capture started!");
}

void stopCapture() {
  if (currentState == CAPTURING) {
    currentState = STOPPED;
    TARGET_SERIAL.end();

    // Close file
    if (dataFile) {
      dataFile.close();
    }

    DEBUG_SERIAL.println("Capture stopped.");
    printStatus();
  } else {
    DEBUG_SERIAL.println("Not currently capturing.");
  }
}

void newCaptureFile() {
  // Generate unique filename
  int fileNum = 0;
  do {
    currentFilename = "capture_" + String(fileNum) + ".csv";
    fileNum++;
  } while (SD.exists(currentFilename.c_str()));

  DEBUG_SERIAL.print("Creating new capture file: ");
  DEBUG_SERIAL.println(currentFilename);

  // Open file and write header
  if (sdCardReady) {
    dataFile = SD.open(currentFilename.c_str(), FILE_WRITE);
    if (dataFile) {
      dataFile.println("Timestamp,Direction,Value_Hex,Value_ASCII,Status");
      dataFile.close();
      DEBUG_SERIAL.println("File created successfully.");
    } else {
      DEBUG_SERIAL.println("ERROR: Could not create file.");
    }
  }

  // Reset statistics
  bytesReceived = 0;
  packetsDetected = 0;
}

void clearBuffer() {
  bufferIndex = 0;
  memset(rxBuffer, 0, BUFFER_SIZE);
  DEBUG_SERIAL.println("Buffer cleared.");
}

void printStatus() {
  unsigned long uptime = (millis() - startTime) / 1000;

  DEBUG_SERIAL.println("========================================");
  DEBUG_SERIAL.println("SerialSniffer Status");
  DEBUG_SERIAL.println("========================================");
  DEBUG_SERIAL.print("State: ");
  switch (currentState) {
    case IDLE: DEBUG_SERIAL.println("IDLE"); break;
    case DETECTING_BAUD: DEBUG_SERIAL.println("DETECTING BAUD"); break;
    case AWAITING_MANUAL_BAUD: DEBUG_SERIAL.println("AWAITING MANUAL INPUT"); break;
    case CAPTURING: DEBUG_SERIAL.println("CAPTURING"); break;
    case STOPPED: DEBUG_SERIAL.println("STOPPED"); break;
  }
  DEBUG_SERIAL.print("Baud Rate: ");
  DEBUG_SERIAL.println(detectedBaud > 0 ? String(detectedBaud) : "Not detected");
  DEBUG_SERIAL.print("Capture File: ");
  DEBUG_SERIAL.println(currentFilename.length() > 0 ? currentFilename : "None");
  DEBUG_SERIAL.print("Bytes Received: ");
  DEBUG_SERIAL.println(bytesReceived);
  DEBUG_SERIAL.print("Buffer Usage: ");
  DEBUG_SERIAL.print(bufferIndex);
  DEBUG_SERIAL.print("/");
  DEBUG_SERIAL.println(BUFFER_SIZE);
  DEBUG_SERIAL.print("SD Card: ");
  DEBUG_SERIAL.println(sdCardReady ? "Ready" : "Not available");
  DEBUG_SERIAL.print("Uptime: ");
  DEBUG_SERIAL.print(uptime);
  DEBUG_SERIAL.println(" seconds");
  DEBUG_SERIAL.println("========================================");
}

// ISR for edge detection
void edgeDetectionISR() {
  if (edgeCount < 50) {
    edgeTimes[edgeCount++] = micros();
  }
}

void detectBaudRate() {
  const int SAMPLE_COUNT = 50;
  const int TIMEOUT_MS = 10000;  // 10 second timeout
  const int MIN_SAMPLES_REQUIRED = 10;

  // Reset edge detection
  edgeCount = 0;

  // Detach from UART hardware temporarily
  TARGET_SERIAL.end();

  // Configure Pin 0 (Serial1 RX) as input with interrupts
  pinMode(0, INPUT);
  attachInterrupt(digitalPinToInterrupt(0), edgeDetectionISR, CHANGE);

  DEBUG_SERIAL.println("Listening for serial transitions...");

  // Wait for samples or timeout
  unsigned long startTime = millis();
  while (edgeCount < SAMPLE_COUNT && (millis() - startTime) < TIMEOUT_MS) {
    delay(10);

    // Progress indicator
    if ((millis() - startTime) % 1000 == 0) {
      DEBUG_SERIAL.print(".");
    }
  }
  DEBUG_SERIAL.println();

  // Detach interrupt
  detachInterrupt(digitalPinToInterrupt(0));

  // Check if we got enough samples
  if (edgeCount < MIN_SAMPLES_REQUIRED) {
    DEBUG_SERIAL.print("Detection failed: Only captured ");
    DEBUG_SERIAL.print(edgeCount);
    DEBUG_SERIAL.println(" edge transitions.");
    promptManualBaudRate();
    return;
  }

  DEBUG_SERIAL.print("Captured ");
  DEBUG_SERIAL.print(edgeCount);
  DEBUG_SERIAL.println(" edge transitions. Analyzing...");

  // Calculate pulse widths between edges
  uint32_t pulseWidths[SAMPLE_COUNT - 1];
  for (int i = 0; i < edgeCount - 1; i++) {
    pulseWidths[i] = edgeTimes[i + 1] - edgeTimes[i];
  }

  // Find shortest consistent pulse (= 1 bit period)
  uint32_t bitPeriod = findShortestConsistentPulse(pulseWidths, edgeCount - 1);

  if (bitPeriod == 0) {
    DEBUG_SERIAL.println("Detection failed: No consistent pulse pattern found.");
    promptManualBaudRate();
    return;
  }

  // Calculate raw baud rate
  uint32_t rawBaud = 1000000 / bitPeriod;
  DEBUG_SERIAL.print("Calculated baud rate: ");
  DEBUG_SERIAL.println(rawBaud);

  // Round to standard baud rate
  uint32_t newBaud = roundToStandardBaud(rawBaud);

  if (newBaud == 0) {
    DEBUG_SERIAL.print("Warning: Calculated baud (");
    DEBUG_SERIAL.print(rawBaud);
    DEBUG_SERIAL.println(") doesn't match standard rates.");
    promptManualBaudRate();
    return;
  }

  // Attempt to validate the detected baud rate
  detectedBaud = newBaud;
  TARGET_SERIAL.begin(detectedBaud);

  DEBUG_SERIAL.print("Testing baud rate ");
  DEBUG_SERIAL.print(detectedBaud);
  DEBUG_SERIAL.println("...");

  if (validateBaudRate()) {
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("SUCCESS! Baud rate detected: ");
    DEBUG_SERIAL.println(detectedBaud);
    DEBUG_SERIAL.println();
    currentState = IDLE;
  } else {
    DEBUG_SERIAL.println("Validation failed. Baud rate may be incorrect.");
    promptManualBaudRate();
  }
}

uint32_t findShortestConsistentPulse(uint32_t* pulses, int count) {
  if (count < 3) return 0;

  // Find minimum pulse width
  uint32_t minPulse = pulses[0];
  for (int i = 1; i < count; i++) {
    if (pulses[i] < minPulse && pulses[i] > 5) {  // Ignore very short (noise)
      minPulse = pulses[i];
    }
  }

  // Count how many pulses are within 10% of minimum
  int matchCount = 0;
  uint32_t tolerance = minPulse / 10;  // 10% tolerance

  for (int i = 0; i < count; i++) {
    if (pulses[i] >= (minPulse - tolerance) && pulses[i] <= (minPulse + tolerance)) {
      matchCount++;
    }
  }

  // Need at least 3 matching pulses to be "consistent"
  if (matchCount >= 3) {
    return minPulse;
  }

  return 0;
}

uint32_t roundToStandardBaud(uint32_t rawBaud) {
  const uint32_t rates[] = {9600, 19200, 38400, 57600, 115200};
  const int numRates = 5;

  uint32_t closest = rates[0];
  uint32_t minDiff = abs((int32_t)rawBaud - (int32_t)closest);

  for (int i = 1; i < numRates; i++) {
    uint32_t diff = abs((int32_t)rawBaud - (int32_t)rates[i]);
    if (diff < minDiff) {
      minDiff = diff;
      closest = rates[i];
    }
  }

  // Only accept if within 5% of standard rate
  uint32_t tolerance = closest / 20;  // 5%
  if (minDiff < tolerance) {
    return closest;
  }

  return 0;  // Not close enough to any standard rate
}

bool validateBaudRate() {
  delay(100);  // Wait for data

  int validCount = 0;
  int totalCount = 0;
  unsigned long start = millis();

  // Try to read 20 bytes or timeout after 1 second
  while (totalCount < 20 && (millis() - start) < 1000) {
    if (TARGET_SERIAL.available()) {
      uint8_t b = TARGET_SERIAL.read();
      totalCount++;

      // Consider valid if: printable ASCII, or common control chars
      if (isPrintable(b) || b == 0x0A || b == 0x0D || b == 0x00) {
        validCount++;
      }
    }
    delay(10);
  }

  // Need at least 5 bytes and 50% validity
  return (totalCount >= 5 && validCount >= (totalCount / 2));
}

void promptManualBaudRate() {
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println("========================================");
  DEBUG_SERIAL.println("Manual Baud Rate Selection");
  DEBUG_SERIAL.println("========================================");
  DEBUG_SERIAL.println("Please select baud rate:");
  DEBUG_SERIAL.println("  1 - 9600");
  DEBUG_SERIAL.println("  2 - 19200");
  DEBUG_SERIAL.println("  3 - 38400");
  DEBUG_SERIAL.println("  4 - 57600");
  DEBUG_SERIAL.println("  5 - 115200");
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.print("Enter selection (1-5): ");

  currentState = AWAITING_MANUAL_BAUD;
}

void handleManualBaudInput(char input) {
  const uint32_t rates[] = {9600, 19200, 38400, 57600, 115200};

  if (input >= '1' && input <= '5') {
    detectedBaud = rates[input - '1'];

    DEBUG_SERIAL.println(input);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("Baud rate set to: ");
    DEBUG_SERIAL.println(detectedBaud);
    DEBUG_SERIAL.println();

    currentState = IDLE;
  } else if (input != '\r' && input != '\n') {
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println("Invalid selection. Please enter 1-5.");
    DEBUG_SERIAL.print("Enter selection (1-5): ");
  }
}

void captureData() {
  static unsigned long lastFlush = 0;
  static unsigned int bytesSinceFlush = 0;
  const unsigned int FLUSH_INTERVAL_MS = 1000;    // Flush every 1 second
  const unsigned int FLUSH_BYTE_COUNT = 512;      // Or every 512 bytes

  if (TARGET_SERIAL.available()) {
    uint8_t incomingByte = TARGET_SERIAL.read();
    bytesReceived++;

    // Add to buffer
    if (bufferIndex < BUFFER_SIZE) {
      rxBuffer[bufferIndex++] = incomingByte;
    } else {
      // Buffer overflow warning
      DEBUG_SERIAL.println("WARNING: Buffer overflow!");
    }

    // Log to SD card (file already open from startCapture)
    if (dataFile) {
      unsigned long timestamp = millis() - startTime;
      dataFile.print(timestamp);
      dataFile.print(",RX,0x");
      if (incomingByte < 0x10) dataFile.print("0"); // Pad single hex digits
      dataFile.print(incomingByte, HEX);
      dataFile.print(",");
      dataFile.print(isPrintable(incomingByte) ? String((char)incomingByte) : ".");
      dataFile.println(",OK");

      bytesSinceFlush++;

      // Periodic flush to ensure data is written to SD card
      unsigned long now = millis();
      if ((now - lastFlush > FLUSH_INTERVAL_MS) || (bytesSinceFlush >= FLUSH_BYTE_COUNT)) {
        dataFile.flush();
        lastFlush = now;
        bytesSinceFlush = 0;
      }
    }

    // Optional: Echo to debug serial (comment out for high-speed capture)
    // Uncomment the lines below for real-time monitoring (will reduce max capture speed)
    /*
    DEBUG_SERIAL.print("RX: 0x");
    if (incomingByte < 0x10) DEBUG_SERIAL.print("0");
    DEBUG_SERIAL.print(incomingByte, HEX);
    DEBUG_SERIAL.print(" (");
    DEBUG_SERIAL.print(isPrintable(incomingByte) ? String((char)incomingByte) : ".");
    DEBUG_SERIAL.println(")");
    */
  }
}

void blinkLED() {
  static unsigned long lastBlink = 0;
  static bool ledState = false;

  if (millis() - lastBlink > 500) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastBlink = millis();
  }
}

void printSDCardInfo() {
  // TODO: Add more detailed SD card information
  DEBUG_SERIAL.println("MicroSD");
}

bool isPrintable(uint8_t c) {
  return (c >= 32 && c <= 126);
}
