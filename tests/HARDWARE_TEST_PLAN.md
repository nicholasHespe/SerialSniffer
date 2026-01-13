# SerialSniffer Hardware Test Plan

This document outlines all tests to be performed once hardware is connected.

## Test Equipment Required

- Teensy 4.1 with SerialSniffer firmware
- MicroSD card (formatted FAT32, 1GB+)
- USB cable for programming and monitoring
- Target serial device(s) for testing:
  - Arduino (any model) OR
  - USB-to-Serial adapter OR
  - Another Teensy/microcontroller
- Jumper wires
- Serial terminal software (Arduino Serial Monitor, PuTTY, etc.)

## Test Environment Setup

```
Test Setup:
  PC (Serial Monitor @ 115200)
    |
    USB
    |
  Teensy 4.1 (SerialSniffer)
    |
    Pin 0 (Serial1 RX) ----> Target Device TX
    Pin 1 (Serial1 TX) ----> Target Device RX
    GND               ----> Target Device GND
```

---

## Phase 1: Basic Functionality Tests

### Test 1.1: Power-On and Initialization
**Objective:** Verify basic startup functionality

**Steps:**
1. Insert formatted MicroSD card into Teensy 4.1
2. Connect Teensy to PC via USB
3. Open Serial Monitor (115200 baud)
4. Press reset button on Teensy

**Expected Results:**
- [ ] Welcome banner displays: "SerialSniffer v0.1.0"
- [ ] SD card initialization shows "OK"
- [ ] Default baud rate shows as 9600
- [ ] Command menu displays
- [ ] No errors printed

**Actual Results:**
```
[Record results here]
```

---

### Test 1.2: Command Interface
**Objective:** Verify all commands work

**Steps:**
Test each command without hardware connected:

1. Press `h` - Show help menu
2. Press `i` - Show status
3. Press `c` - Clear buffer
4. Press `n` - New capture file

**Expected Results:**
- [ ] `h` - Help menu displays all commands
- [ ] `i` - Status shows: IDLE state, baud 9600, no file, 0 bytes, SD OK
- [ ] `c` - "Buffer cleared" message
- [ ] `n` - "Creating new capture file: capture_0.csv" message
- [ ] File created on SD card with CSV header

**Actual Results:**
```
[Record results here]
```

---

### Test 1.3: SD Card File Creation
**Objective:** Verify SD card writing works

**Steps:**
1. Press `n` to create new file
2. Check status with `i`
3. Remove SD card
4. Insert into PC and check files

**Expected Results:**
- [ ] File `capture_0.csv` exists on SD card
- [ ] File contains CSV header: "Timestamp,Direction,Value_Hex,Value_ASCII,Status"
- [ ] Multiple `n` commands create capture_1.csv, capture_2.csv, etc.

**Actual Results:**
```
[Record results here]
```

---

## Phase 2: Baud Rate Detection Tests

### Test 2.1: Detection at 9600 Baud
**Objective:** Test auto-detection at 9600 baud

**Test Device Setup:**
- Configure target device to transmit at 9600 baud
- Connect continuously sending data (e.g., "Hello\r\n" repeatedly)

**Steps:**
1. Connect target device (9600 baud, transmitting)
2. Press `d` to start detection
3. Wait up to 10 seconds
4. Observe detection result

**Expected Results:**
- [ ] Detection completes in < 10 seconds
- [ ] Detected baud rate: 9600
- [ ] Message: "Baud rate detected: 9600"
- [ ] Status (`i`) shows "Baud Rate: 9600 (Auto-detected)"

**Actual Results:**
```
Detected baud: _______
Detection time: _______ seconds
Notes:
```

---

### Test 2.2: Detection at 19200 Baud
**Objective:** Test auto-detection at 19200 baud

**Test Device Setup:**
- Configure target device to 19200 baud

**Steps:**
1. Connect target device (19200 baud, transmitting)
2. Press `d` to start detection
3. Wait up to 10 seconds

**Expected Results:**
- [ ] Detected baud rate: 19200
- [ ] Detection successful within 10 seconds

**Actual Results:**
```
Detected baud: _______
Detection time: _______ seconds
```

---

### Test 2.3: Detection at 38400 Baud
**Objective:** Test auto-detection at 38400 baud

**Steps:**
[Same as Test 2.2 but at 38400]

**Expected Results:**
- [ ] Detected baud rate: 38400

**Actual Results:**
```
Detected baud: _______
Detection time: _______ seconds
```

---

### Test 2.4: Detection at 57600 Baud
**Objective:** Test auto-detection at 57600 baud

**Steps:**
[Same as Test 2.2 but at 57600]

**Expected Results:**
- [ ] Detected baud rate: 57600

**Actual Results:**
```
Detected baud: _______
Detection time: _______ seconds
```

---

### Test 2.5: Detection at 115200 Baud
**Objective:** Test auto-detection at 115200 baud (highest standard rate)

**Steps:**
[Same as Test 2.2 but at 115200]

**Expected Results:**
- [ ] Detected baud rate: 115200

**Actual Results:**
```
Detected baud: _______
Detection time: _______ seconds
```

---

### Test 2.6: Detection Timeout (No Signal)
**Objective:** Test timeout when no signal present

**Steps:**
1. Disconnect all wires from Serial1 RX (Pin 0)
2. Press `d` to start detection
3. Wait 10 seconds

**Expected Results:**
- [ ] Detection times out after 10 seconds
- [ ] Message: "Auto-detection failed!"
- [ ] Manual baud rate menu displays
- [ ] Options 1-5 shown for manual selection

**Actual Results:**
```
Timeout occurred: Yes/No
Time to timeout: _______ seconds
Manual menu displayed: Yes/No
```

---

### Test 2.7: Detection with Noisy Signal
**Objective:** Test detection with intermittent/noisy signal

**Steps:**
1. Connect target device
2. Physically tap/wiggle RX wire during detection
3. Press `d` to start detection

**Expected Results:**
- [ ] Either detects correctly (if enough good signal)
- [ ] OR times out and prompts manual input
- [ ] No crashes or hangs

**Actual Results:**
```
[Record results]
```

---

### Test 2.8: Manual Baud Rate Selection
**Objective:** Test manual baud rate input

**Steps:**
1. Trigger manual input (either via failed detection or `b` command)
2. Press `1` (9600)
3. Verify baud rate set
4. Repeat with `2`, `3`, `4`, `5`

**Expected Results:**
- [ ] `1` sets 9600 baud
- [ ] `2` sets 19200 baud
- [ ] `3` sets 38400 baud
- [ ] `4` sets 57600 baud
- [ ] `5` sets 115200 baud
- [ ] Invalid input (e.g., `6`, `a`) shows error message

**Actual Results:**
```
[Record results for each selection]
```

---

## Phase 3: Data Capture Tests

### Test 3.1: Basic Data Capture at 9600 Baud
**Objective:** Verify data capture functionality

**Test Device Setup:**
- Target sending: "Hello World\r\n" every 1 second at 9600 baud

**Steps:**
1. Ensure baud rate detected/set to 9600
2. Press `n` to create new capture file
3. Press `s` to start capture
4. Wait 10 seconds
5. Press `t` to stop capture
6. Check status with `i`
7. Remove SD card and check file

**Expected Results:**
- [ ] Capture starts successfully
- [ ] LED blinks during capture (indicates activity)
- [ ] Status shows "CAPTURING" state
- [ ] After stop: Status shows bytes received count
- [ ] CSV file contains captured data with timestamps
- [ ] Hex values match ASCII characters (0x48='H', 0x65='e', etc.)
- [ ] Timestamps increase monotonically

**Actual Results:**
```
Bytes captured: _______
Sample CSV lines:
[Paste first 5 lines of CSV here]
```

---

### Test 3.2: Sustained Capture at 115200 Baud
**Objective:** Test high-speed continuous capture

**Test Device Setup:**
- Target continuously transmitting at 115200 baud (no delays)

**Steps:**
1. Set/detect baud rate to 115200
2. Start capture
3. Run for 1 minute
4. Stop capture
5. Check for data loss

**Expected Results:**
- [ ] No "WARNING: Buffer overflow!" messages
- [ ] Byte count reasonable (expect ~11,520 bytes/sec)
- [ ] No gaps in timestamps
- [ ] All data captured correctly
- [ ] SD card file valid and readable

**Expected byte count:** ~691,200 bytes (11,520 bytes/sec × 60 sec)

**Actual Results:**
```
Duration: _______ seconds
Bytes captured: _______
Calculated rate: _______ bytes/sec
Data loss detected: Yes/No
Buffer overflows: _______
```

---

### Test 3.3: Multiple Capture Sessions
**Objective:** Test multiple sequential captures

**Steps:**
1. Create new file with `n` → capture_0.csv
2. Start capture with `s`
3. Capture for 10 seconds
4. Stop with `t`
5. Create new file with `n` → capture_1.csv
6. Start capture with `s`
7. Capture for 10 seconds
8. Stop with `t`
9. Check both files

**Expected Results:**
- [ ] Both files created successfully
- [ ] capture_0.csv contains first session data
- [ ] capture_1.csv contains second session data
- [ ] No data mixing between files
- [ ] Both files have valid headers

**Actual Results:**
```
[Record results]
```

---

### Test 3.4: Continuous Capture (24 hours)
**Objective:** Test long-term reliability

**Test Device Setup:**
- Target sending continuous data at 9600 baud

**Steps:**
1. Start capture
2. Let run for 24 hours
3. Check status periodically
4. Stop and check results

**Expected Results:**
- [ ] No crashes or hangs
- [ ] Continuous data capture
- [ ] File size reasonable (~1.6 MB for 24h at 9600 baud)
- [ ] No memory leaks (check status over time)
- [ ] Buffer usage remains stable

**Actual Results:**
```
Duration: _______ hours
Bytes captured: _______
File size: _______ MB
System stable: Yes/No
Issues encountered:
```

---

### Test 3.5: Buffer Overflow Handling
**Objective:** Test buffer full condition

**Test Device Setup:**
- Target sending VERY fast (faster than SD can write)
- OR artificially fill buffer before capture

**Steps:**
1. Fill buffer to near capacity
2. Start high-speed capture
3. Monitor for overflow warnings

**Expected Results:**
- [ ] "WARNING: Buffer overflow!" message appears
- [ ] System doesn't crash
- [ ] Capture continues after overflow
- [ ] Overflow logged in status

**Actual Results:**
```
[Record results]
```

---

## Phase 4: Data Validation Tests

### Test 4.1: Hex Format Validation
**Objective:** Verify hex values are correct

**Test Data:** Send known bytes: 0x00, 0x0F, 0x10, 0xFF

**Steps:**
1. Configure target to send test bytes
2. Capture data
3. Compare CSV hex values to expected

**Expected Results:**
- [ ] 0x00 recorded as "0x00" (with padding)
- [ ] 0x0F recorded as "0x0F" (with padding)
- [ ] 0x10 recorded as "0x10"
- [ ] 0xFF recorded as "0xFF"

**Actual Results:**
```
CSV output for test bytes:
```

---

### Test 4.2: ASCII Character Mapping
**Objective:** Verify ASCII column correctness

**Test Data:** Send "ABC123!@#"

**Steps:**
1. Capture known ASCII string
2. Check CSV ASCII column

**Expected Results:**
- [ ] Printable chars displayed correctly (A, B, C, 1, 2, 3, !, @, #)
- [ ] Control chars show as "." (e.g., 0x00, 0x0D, 0x0A)

**Actual Results:**
```
CSV ASCII column output:
```

---

### Test 4.3: Timestamp Accuracy
**Objective:** Verify timestamp precision

**Test Device Setup:**
- Target sending 1 byte every 100ms (known timing)

**Steps:**
1. Capture data with known timing
2. Calculate timestamp differences
3. Compare to expected 100ms intervals

**Expected Results:**
- [ ] Timestamps approximately 100ms apart (±10ms)
- [ ] Timestamps monotonically increasing
- [ ] No negative timestamps

**Actual Results:**
```
Average interval: _______ ms
Min interval: _______ ms
Max interval: _______ ms
```

---

## Phase 5: Edge Case Tests

### Test 5.1: SD Card Removed During Capture
**Objective:** Test behavior when SD card fails

**Steps:**
1. Start capture
2. Remove SD card during capture
3. Observe behavior

**Expected Results:**
- [ ] Error message displayed
- [ ] System doesn't crash
- [ ] Can stop capture gracefully
- [ ] Can resume after reinserting card

**Actual Results:**
```
[Record behavior]
```

---

### Test 5.2: SD Card Full
**Objective:** Test behavior when SD card fills up

**Steps:**
1. Use nearly-full SD card
2. Start capture
3. Wait for card to fill

**Expected Results:**
- [ ] Error message when full
- [ ] No data corruption
- [ ] System remains responsive
- [ ] Can stop capture

**Actual Results:**
```
[Record behavior]
```

---

### Test 5.3: Rapid Command Entry
**Objective:** Test command handling under stress

**Steps:**
1. Rapidly press multiple commands: s, t, s, t, i, c, n, n, n
2. Observe system response

**Expected Results:**
- [ ] System handles all commands
- [ ] No crashes or hangs
- [ ] Appropriate error messages for invalid sequences

**Actual Results:**
```
[Record behavior]
```

---

### Test 5.4: Capture Without Detection
**Objective:** Test default baud rate usage

**Steps:**
1. Power on Teensy (defaults to 9600)
2. Without running detection, press `s` to start capture
3. Connect target at 9600 baud

**Expected Results:**
- [ ] Capture starts with 9600 baud
- [ ] Data captured correctly
- [ ] Warning shown that baud not detected (optional)

**Actual Results:**
```
[Record results]
```

---

## Phase 6: Integration Tests

### Test 6.1: Full Workflow - Unknown Device
**Objective:** Simulate real-world usage with unknown device

**Scenario:** User connects unknown serial device

**Steps:**
1. Power on Teensy (defaults to 9600)
2. Connect unknown device (you set it to 57600 but pretend not to know)
3. Press `d` to detect baud rate
4. Verify detection result
5. Press `n` to create new file
6. Press `s` to start capture
7. Capture for 30 seconds
8. Press `t` to stop
9. Remove SD card and analyze data

**Expected Results:**
- [ ] Baud rate detected as 57600
- [ ] Data captured successfully
- [ ] CSV file valid and analyzable

**Actual Results:**
```
[Record complete workflow results]
```

---

### Test 6.2: Device Change Workflow
**Objective:** Test switching between different devices

**Steps:**
1. Capture from Device A at 9600 baud
2. Stop capture
3. Disconnect Device A
4. Connect Device B at 115200 baud
5. Run detection with `d`
6. Start new capture

**Expected Results:**
- [ ] Detection finds new baud rate (115200)
- [ ] New capture uses correct baud rate
- [ ] Previous capture file intact
- [ ] No data mixing

**Actual Results:**
```
[Record results]
```

---

## Phase 7: Python Analysis Integration Tests

### Test 7.1: Python Tool Can Read Capture Files
**Objective:** Verify Python analysis suite works with captured data

**Steps:**
1. Capture real data to CSV file
2. Copy file to PC
3. Run: `serialsniffer stats capture_0.csv`
4. Run: `serialsniffer analyze capture_0.csv`

**Expected Results:**
- [ ] Python tool loads file without errors
- [ ] Statistics displayed correctly
- [ ] Analysis runs successfully

**Actual Results:**
```
[Record Python tool output]
```

---

## Test Results Summary

### Overall Test Statistics

| Phase | Tests Passed | Tests Failed | Pass Rate |
|-------|--------------|--------------|-----------|
| Phase 1: Basic | __/3 | __/3 | __% |
| Phase 2: Baud Detection | __/8 | __/8 | __% |
| Phase 3: Data Capture | __/5 | __/5 | __% |
| Phase 4: Data Validation | __/3 | __/3 | __% |
| Phase 5: Edge Cases | __/4 | __/4 | __% |
| Phase 6: Integration | __/2 | __/2 | __% |
| Phase 7: Python Integration | __/1 | __/1 | __% |
| **TOTAL** | **__/26** | **__/26** | **__%** |

### Critical Issues Found
```
[List any critical bugs that prevent basic functionality]

1.
2.
3.
```

### Non-Critical Issues Found
```
[List minor bugs or improvements needed]

1.
2.
3.
```

### Performance Metrics

| Metric | Target | Actual | Pass/Fail |
|--------|--------|--------|-----------|
| Max sustained baud rate | 115200 | _____ | ___ |
| Data loss at 115200 | < 0.1% | ____% | ___ |
| Detection time (9600) | < 10 sec | ___sec | ___ |
| Detection time (115200) | < 10 sec | ___sec | ___ |
| 24-hour stability | No crashes | _____ | ___ |

### Hardware Configuration Used
```
Teensy 4.1 Firmware Version: v0.1.0
SD Card: [Brand/Model/Size]
Target Device(s): [List devices used for testing]
Test Date: [Date]
Tester: [Name]
```

---

## Notes and Observations
```
[Add any additional notes, observations, or recommendations]
```

---

## Recommended Test Order

**Quick Smoke Test (15 minutes):**
- Test 1.1, 1.2, 1.3, 2.1, 3.1

**Full Functional Test (2 hours):**
- All Phase 1, 2, 3, 4 tests

**Extended Reliability Test (24+ hours):**
- Test 3.4 (24-hour capture)

**Complete Test Suite:**
- All tests in order (est. 26+ hours including long-running tests)
