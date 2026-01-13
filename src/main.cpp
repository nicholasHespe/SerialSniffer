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
  CAPTURING,
  STOPPED
};
CaptureState currentState = IDLE;

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
      // TODO: Implement baud rate detection
      detectBaudRate();
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
  DEBUG_SERIAL.println("  s - Start capture (auto-detect baud)");
  DEBUG_SERIAL.println("  t - Stop capture");
  DEBUG_SERIAL.println("  n - New capture file");
  DEBUG_SERIAL.println("  c - Clear buffer");
  DEBUG_SERIAL.println("  i - Show status/info");
  DEBUG_SERIAL.println("  h - Show this help menu");
  DEBUG_SERIAL.println();
}

void handleCommand() {
  char cmd = DEBUG_SERIAL.read();

  switch (cmd) {
    case 's':
    case 'S':
      startCapture();
      break;

    case 't':
    case 'T':
      stopCapture();
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

void detectBaudRate() {
  // TODO: Implement automatic baud rate detection algorithm
  // For now, this is a placeholder
  currentState = CAPTURING;
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
