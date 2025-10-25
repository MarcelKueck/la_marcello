# Heat Control System Setup Guide

**Project:** La Marcello Coffee Roaster - AC Dimmer Heat Control  
**Date:** October 25, 2025  
**Author:** Marcel

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Electrical Fundamentals](#electrical-fundamentals)
3. [Heat Coil Analysis](#heat-coil-analysis)
4. [AC Dimmer Module](#ac-dimmer-module)
5. [Wiring Diagram](#wiring-diagram)
6. [Zero-Cross Detection & Phase Control](#zero-cross-detection--phase-control)
7. [Safety Considerations](#safety-considerations)
8. [Next Steps](#next-steps)

---

## Project Overview

### Goal
Build a PID-controlled coffee roaster using:
- **Temperature sensing:** MAX31855 thermocouple (already working)
- **Heat control:** RobotDyn AC Dimmer module controlling popcorn maker heat coil
- **Controller:** ESP32 microcontroller

### System Architecture
```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│ Thermocouple│─────►│    ESP32    │─────►│ AC Dimmer   │
│  (Sensor)   │      │ (PID Logic) │      │  (Actuator) │
└─────────────┘      └─────────────┘      └─────────────┘
                                                  │
                                                  ▼
                                           ┌─────────────┐
                                           │  Heat Coil  │
                                           └─────────────┘
```

---

## Electrical Fundamentals

### Ohm's Law
The foundation of electrical circuits. It relates three fundamental quantities:

**V = I × R**

Where:
- **V** = Voltage (Volts) - electrical pressure/potential
- **I** = Current (Amperes) - flow of electrons
- **R** = Resistance (Ohms, Ω) - opposition to current flow

#### Derived Formulas:
- **I = V / R** (Current equals Voltage divided by Resistance)
- **R = V / I** (Resistance equals Voltage divided by Current)

### Power (Electrical Energy)
Power is the rate at which electrical energy is consumed or converted to heat.

**P = V × I**  (Power = Voltage × Current)

Using Ohm's Law, we can also express power as:
- **P = V² / R** (when you know voltage and resistance)
- **P = I² × R** (when you know current and resistance)

Where:
- **P** = Power (Watts, W)

### Example Calculation (Our Heat Coil):
- Voltage (V) = 230V AC (European mains)
- Total Resistance (R) = 50Ω
- Power (P) = V² / R = 230² / 50 = 52,900 / 50 = **1,058W ≈ 1kW**

This means the heat coil consumes about **1 kilowatt** of power when running at full capacity.

### AC vs DC
- **DC (Direct Current):** Flows in one direction (batteries, USB power)
- **AC (Alternating Current):** Reverses direction periodically (mains power)
  - In Europe/Netherlands: **230V, 50Hz**
  - Frequency (50Hz) means the current reverses direction **50 times per second**
  - Each cycle has 2 **zero crossings** → **100 zero crossings per second**

---

## Heat Coil Analysis

### Physical Setup
The popcorn maker heat coil assembly has **3 wires:**
- Red wire
- White/Grey wire  
- Blue wire

### Resistance Measurements
**Measurements taken (unplugged, using multimeter):**

| Wire Pair | Resistance |
|-----------|------------|
| Red ↔ White/Grey | 42Ω |
| Red ↔ Blue | 50Ω |
| Blue ↔ White/Grey | 8Ω |

### Analysis
Notice that: **42Ω + 8Ω = 50Ω**

This reveals the internal circuit structure:

```
Red ──[Thermal Fuse: 8Ω]── White/Grey ──[Heat Coil: 42Ω]── Blue
```

**Components:**
1. **Thermal Fuse** (between Red and White/Grey)
   - Resistance: ~8Ω
   - Function: Safety device - permanently breaks circuit if overheated
   - Typically rated for 184°C - 240°C
   
2. **Heating Element** (between White/Grey and Blue)
   - Resistance: 42Ω
   - This is the actual resistive wire that produces heat

3. **White/Grey Wire**
   - Internal connection point (tap) between thermal fuse and heating element
   - **Not used in our application** - leave disconnected

### Power Rating
- **Total resistance:** 50Ω (thermal fuse + heating element in series)
- **Voltage:** 230V AC (Netherlands/European mains)
- **Power:** P = V² / R = 230² / 50 = **1,058W ≈ 1kW**

### Current Draw
Using Ohm's Law: I = V / R
- **Current:** I = 230V / 50Ω = **4.6A**

This is well within the **8A rating** of the RobotDyn dimmer module ✓

---

## AC Dimmer Module

### Module: RobotDyn AC Dimmer
- **Model:** 1 Channel, 8A, 3.3V/5V Logic
- **Website:** rbdimmer.com
- **Type:** Phase-angle control (TRIAC-based)
- **Frequency:** 50/60Hz AC

### Pin Configuration

#### Low Voltage Side (Safe - connects to ESP32):
| Pin | Function | ESP32 Connection |
|-----|----------|------------------|
| VCC | Power Supply | 3.3V |
| GND | Ground | GND |
| Z-C | Zero-Cross Detection Output | GPIO 19 |
| PWM | Control Signal Input | GPIO 18 |

#### High Voltage Side (⚠️ DANGEROUS - Mains AC):
| Pin | Function | Connection |
|-----|----------|------------|
| AC-IN L | Live Input | Mains Live (230V) |
| AC-IN N | Neutral Input | Mains Neutral |
| AC-OUT L | Live Output | Heat Coil Red Wire |
| AC-OUT N | Neutral Output | Heat Coil Blue Wire |

### How It Works

The dimmer uses **phase-angle control** with a TRIAC (Triode for Alternating Current):

1. **Zero-Cross Detection (Z-C pin):**
   - Module detects when AC voltage crosses zero
   - Sends a pulse to ESP32 on Z-C pin
   - Happens **100 times/second** for 50Hz AC (2 crossings per cycle)

2. **Phase Angle Control (PWM pin):**
   - NOT traditional PWM! Despite the pin name.
   - ESP32 sends a trigger pulse AFTER each zero crossing
   - Delay between zero-cross and trigger determines power
   - Longer delay = less power
   - Shorter delay = more power

### Power Control Principle

```
AC Sine Wave (50Hz):

Full Power (0% delay):
    ████████████████████        ████████████████████
   ╱                    ╲      ╱                    ╲
──╱                      ╲────╱                      ╲──
  ↑ trigger immediately   ↑ trigger immediately
  
50% Power (mid delay):
        ████████                    ████████
   ╱            ╲              ╱            ╲
──╱              ╲────────────╱              ╲──────
  ↑ wait ~5ms     ↑            ↑ wait ~5ms
  
0% Power (max delay or no trigger):
                                
   ╱            ╲              ╱            ╲
──╱              ╲────────────╱              ╲──────
  ↑ no trigger    ↑            ↑ no trigger
```

### Timing Calculations (50Hz)
- **AC Period:** T = 1/50Hz = 20ms (full cycle)
- **Half Period:** 10ms (one zero crossing to next)
- **Control Range:** 0-10ms delay after zero crossing
  - 0ms delay = 100% power (full sine wave)
  - 5ms delay = ~50% power (half sine wave)
  - 10ms delay = 0% power (no conduction)

---

## Wiring Diagram

### Complete System Connection

```
                    ╔═════════════════╗
  Mains 230V L ─────╢ AC-IN L         ║
                    ║                 ║
  Mains 230V N ─────╢ AC-IN N         ║
                    ║   AC DIMMER     ║
                    ║   RobotDyn 8A   ║
  Red Wire ◄────────╢ AC-OUT L        ║
  (Heat Coil)       ║                 ║
  Blue Wire ◄───────╢ AC-OUT N        ║
  (Heat Coil)       ╚═════════════════╝
                         ║  ║  ║  ║
                        PWM ZC VCC GND
                         ║  ║  ║  ║
                    ┌────╨──╨──╨──╨────┐
                    │  GPIO18 GPIO19   │
                    │                  │
                    │      ESP32       │───── USB (Programming/Power)
                    │                  │
                    │   3.3V    GND    │
                    └──────────────────┘
                         │      │
                    ┌────┴──────┴────┐
                    │   MAX31855      │
                    │  Thermocouple   │
                    │    Module       │
                    └─────────────────┘
                          │
                    K-Type Thermocouple
                    (Temperature Sensor)

Heat Coil Internal Structure:
  Red ──[Thermal Fuse 8Ω]── White/Grey ──[Heat Coil 42Ω]── Blue
                              (not used)
```

### Wire Connections Summary

**ESP32 Connections:**
- GPIO 5 → MAX31855 CS (Chip Select) - *already configured*
- GPIO 18 → AC Dimmer PWM (Control Signal)
- GPIO 19 ← AC Dimmer Z-C (Zero Cross Detection)
- 3.3V → AC Dimmer VCC + MAX31855 VCC
- GND → AC Dimmer GND + MAX31855 GND
- SPI pins → MAX31855 (MISO, MOSI, SCK) - *already configured*

**AC Dimmer Connections:**
- AC-IN L ← Mains Live (230V) ⚠️
- AC-IN N ← Mains Neutral ⚠️
- AC-OUT L → Heat Coil Red Wire
- AC-OUT N → Heat Coil Blue Wire

**Heat Coil:**
- Red Wire → AC Dimmer OUT-L
- Blue Wire → AC Dimmer OUT-N
- White/Grey Wire → **Leave disconnected**

---

## Zero-Cross Detection & Phase Control

### Why Zero-Cross Detection?

**Benefits:**
1. **Reduced EMI (Electromagnetic Interference):** Switching at zero voltage minimizes electrical noise
2. **Longer TRIAC Life:** Less stress on switching component
3. **Precise Power Control:** Timing reference for phase angle control

### How It Works

**1. Zero-Cross Detection (Hardware):**
The dimmer module has an optocoupler that monitors AC voltage:
```
When AC voltage ≈ 0V → Z-C pin goes HIGH (pulse to ESP32)
```

**2. Interrupt Service Routine (Software):**
```cpp
// ESP32 detects Z-C pulse via interrupt on GPIO 19
void IRAM_ATTR zeroCrossISR() {
    // Record time of zero crossing
    // Start timer for firing delay
}
```

**3. Firing Delay Calculation:**
```cpp
// For 50Hz AC:
// Half period = 10ms = 10,000 microseconds
// Power % → Delay calculation
// 100% power = 0μs delay
// 0% power = 10,000μs delay (or don't trigger)

delay_us = (10000 * (100 - power_percent)) / 100;
```

**4. TRIAC Trigger:**
```cpp
// After calculated delay, send short pulse on GPIO 18
digitalWrite(PWM_PIN, HIGH);
delayMicroseconds(10); // Short pulse
digitalWrite(PWM_PIN, LOW);
```

### Frequency Considerations

**Netherlands/Europe:** 50Hz
- Period: 20ms
- Half-period: 10ms (10,000μs)
- Zero crossings: 100/second

**USA/Canada:** 60Hz
- Period: 16.67ms
- Half-period: 8.33ms (8,333μs)
- Zero crossings: 120/second

Our code will be configured for **50Hz**.

---

## Safety Considerations

### ⚠️ HIGH VOLTAGE WARNINGS ⚠️

**YOU ARE WORKING WITH 230V MAINS ELECTRICITY - THIS CAN BE LETHAL!**

### Essential Safety Measures:

1. **Always Disconnect Power:**
   - UNPLUG from mains before any wiring work
   - Use a power switch that's easy to access
   - Consider adding an emergency stop button

2. **Thermal Protection:**
   - ✅ Keep thermal fuse in circuit (8Ω between Red and White/Grey wires)
   - Thermal fuse is a one-time safety device
   - If it blows, replace it - never bypass!

3. **Electrical Isolation:**
   - ESP32 and dimmer control side are isolated from mains via optocouplers
   - Never touch AC side while powered
   - Use insulated enclosure

4. **Proper Wiring:**
   - Use appropriate wire gauge for 5A current
   - Ensure all connections are secure
   - No exposed conductors
   - Strain relief for all cables

5. **Grounding:**
   - Ensure chassis/enclosure is properly grounded
   - Use 3-wire mains cable with earth connection

6. **Software Safety:**
   - Implement watchdog timer
   - Default dimmer to 0% on startup
   - Set maximum temperature limit in code
   - Add timeout protection (if no temp update, shut off)

7. **Testing:**
   - Test code with low voltage first if possible
   - Start with low power settings
   - Monitor temperature closely during initial tests
   - Have fire extinguisher nearby

8. **Environmental:**
   - Ensure good ventilation
   - Keep away from flammable materials
   - Don't leave unattended while powered

### Failure Modes & Protection:

| Failure Scenario | Protection |
|------------------|------------|
| ESP32 crashes | Watchdog timer resets ESP32 |
| Temperature sensor fails | Code detects and shuts down |
| Temperature runaway | Thermal fuse breaks circuit |
| PID output error | Software limits (0-100%) |
| Loss of zero-cross signal | Dimmer won't trigger (safe) |
| Power surge | Thermal fuse + circuit breaker |

---

## Next Steps

### Immediate Tasks:

1. **Hardware Setup:**
   - [ ] Connect ESP32 to AC Dimmer module (low voltage side only)
   - [ ] Test Z-C detection with oscilloscope or logic analyzer (optional)
   - [ ] Verify 3.3V logic levels are working

2. **Software Development:**
   - [ ] Create `ACDimmer.h` header file (class definition)
   - [ ] Create `ACDimmer.cpp` implementation file
   - [ ] Test dimmer control at various power levels (UNPLUGGED from AC first!)
   - [ ] Integrate with existing thermocouple code in `main.cpp`

3. **Testing (Low Voltage First):**
   - [ ] Verify zero-cross interrupts are firing
   - [ ] Test timing accuracy
   - [ ] Validate power level calculations

4. **AC Power Testing (⚠️ DANGER ZONE):**
   - [ ] Wire AC side in proper enclosure
   - [ ] Test at 10% power with heat coil
   - [ ] Gradually increase to 50%, then 100%
   - [ ] Monitor temperature and ensure thermal fuse is functional

5. **PID Controller:**
   - [ ] Implement PID algorithm
   - [ ] Tune PID parameters (Kp, Ki, Kd)
   - [ ] Test temperature setpoint tracking
   - [ ] Create roast profiles

### Learning Goals Achieved So Far:

✅ Understanding Ohm's Law and power calculations  
✅ Analyzing resistance measurements to determine circuit structure  
✅ Understanding AC vs DC and zero-cross detection concept  
✅ Safety awareness for high voltage projects  
✅ System architecture and component integration planning  

### Questions to Think About Before Coding:

1. **What happens if the ESP32 crashes while the heater is at 100% power?**
   - Think about watchdog timers and fail-safe designs

2. **How fast should the PID loop run?**
   - Temperature changes slowly (thermal mass)
   - Too fast → unstable
   - Too slow → poor control

3. **Should we implement open-loop testing first?**
   - Set power level manually before adding PID
   - Validate hardware works correctly

---

## Additional Resources

### Datasheets & References:
- RobotDyn AC Dimmer: rbdimmer.com
- MAX31855 Thermocouple: See `MAX31855_datasheet.pdf`
- ESP32 Technical Reference: [espressif.com](https://www.espressif.com/en/products/socs/esp32)

### Useful Concepts to Research:
- TRIAC operation and phase control
- PID tuning methods (Ziegler-Nichols, etc.)
- Interrupt service routines (ISR) in embedded systems
- Hardware timers on ESP32

### Libraries We'll Use:
- `Adafruit_MAX31855` - Already in use for thermocouple
- Custom AC Dimmer class - We'll create this
- Possible PID library or custom implementation

---

**Next Document:** Code implementation guide for AC dimmer control

**Related Files:**
- `firmware/src/main.cpp` - Current thermocouple reading code
- `docs/ELECTRICAL_ANALYSIS.md` - Component measurements template
- `docs/PLAN.md` - Overall project plan

---

*Stay safe and happy learning! Remember: Safety first, then knowledge, then coffee! ☕*
