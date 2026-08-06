# Construction Workers on the Go (COGO)

COGO is a C++ based wearable safety system integrating an Arduino smartwatch and helmet module equipped with bio-sensors and environmental monitoring. It features real-time hazard alerts, dynamic multi-screen OLED feedback, and proximity detection to enhance safety for construction personnel on-site.

---

## Features

* **Multi-Screen OLED Display**: Automatically cycles through real-time operational data:
  * **Date & Time Screen**: Displays active system runtime and date.
  * **Environmental Sensor Screen**: Displays sound loudness levels (dB warning trigger) and environmental conditions.
  * **Pulse Graph Screen**: Renders a real-time moving pulse wave visualization.
* **Proximity & Distance Laser Toggle**:
  * Push-button toggle activates an ultrasonic distance sensor (`Ping`) paired with a targeting laser to measure and report proximity in inches.
* **Smart Noise Monitoring**:
  * Continually tracks ambient sound levels. 
  * Triggers a red alert LED and safety warnings (*"Wear Ear muffs"*) when loudness exceeds threshold levels.
* **Automatic Ambient Lighting**:
  * Uses a moving average over 10 LDR (Light Dependent Resistor) readings to control automatic headlight/work-light LEDs depending on ambient dark/light transitions.

---

## Pin Layout & Hardware Mapping

| Component / Function | Pin Assignment | Type |
| :--- | :--- | :--- |
| **OLED Display (I2C)** | `SDA` / `SCL` (`0x3C`) | I2C Communication |
| **LDR (Light Sensor)** | `A0` | Analog Input |
| **Loudness Sensor** | `A1` | Analog Input |
| **Pulse Sensor** | `A5` | Analog Input |
| **Mode Toggle Button** | `Pin 2` | Digital Input (Internal Debounced) |
| **Ultrasonic Ping Sensor** | `Pin 7` | Digital I/O |
| **Auto-Light LED** | `Pin 9` | Digital Output |
| **Loudness Alert Red LED** | `Pin 10` | Digital Output |
| **Laser Pointer** | `Pin 13` | Digital Output |

---

## Software & Libraries Required

To compile and upload this project using the Arduino IDE, install the following libraries via the **Library Manager**:

1. `Adafruit_SSD1306` (OLED Display Driver)
2. `Adafruit_GFX` (Graphics Dependency)
3. `Time` / `TimeLib` (Timekeeping functionality)
4. `Wire` (Built-in I2C communication)

---

## Getting Started

1. **Hardware Setup**: Wire the OLED display, LEDs, button, and sensors according to the pin table above.
2. **Install Libraries**: Ensure all required libraries are installed in your Arduino IDE.
3. **Upload Code**: 
   * Connect your Arduino board via USB.
   * Select your target board and port in the Arduino IDE.
   * Upload the `COGO.ino` sketch.
4. **Operation**:
   * Watch the OLED cycle through the default screens every 5 to 30 seconds.
   * Press the toggle button on **Pin 2** to switch into dedicated distance measurement laser mode at any time.
