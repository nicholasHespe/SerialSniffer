/*
 * SerialSniffer - Function Prototypes and Declarations
 *
 * Hardware: Teensy 4.1
 * Author: SerialSniffer Team
 * License: TBD
 */

#ifndef SERIALSNIFFER_H
#define SERIALSNIFFER_H

#include <Arduino.h>

// ==================== Function Prototypes ====================

/**
 * Display the command menu to the debug serial port
 */
void printMenu();

/**
 * Process incoming commands from the debug serial port
 * Handles: s/S (start), t/T (stop), n/N (new file), c/C (clear), i/I (info), h/H (help)
 */
void handleCommand();

/**
 * Start data capture session
 * Initializes baud rate detection and opens log file
 */
void startCapture();

/**
 * Stop active data capture session
 * Closes log file and ends target serial communication
 */
void stopCapture();

/**
 * Create a new capture file on SD card
 * Generates unique filename and writes CSV header
 */
void newCaptureFile();

/**
 * Clear the internal capture buffer
 * Resets buffer index and zeroes buffer memory
 */
void clearBuffer();

/**
 * Display current system status to debug serial
 * Shows: state, baud rate, file, bytes received, buffer usage, SD card status, uptime
 */
void printStatus();

/**
 * Detect baud rate of target serial communication
 * TODO: Implement automatic detection algorithm
 */
void detectBaudRate();

/**
 * Capture incoming data from target serial port
 * Stores data in buffer and logs to SD card
 */
void captureData();

/**
 * Blink the status LED to indicate activity
 * Non-blocking blink at 500ms intervals
 */
void blinkLED();

/**
 * Print SD card information to debug serial
 * Displays card type and capacity
 */
void printSDCardInfo();

/**
 * Check if a byte is a printable ASCII character
 * @param c Byte to check
 * @return true if character is printable (32-126), false otherwise
 */
bool isPrintable(uint8_t c);

#endif // SERIALSNIFFER_H
