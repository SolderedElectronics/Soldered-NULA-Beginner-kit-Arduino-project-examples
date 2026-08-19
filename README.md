<img src="https://soldered.com/cdn/shop/files/333371_featured-photo_6fa868_124b306f-abaf-4553-b5cb-8c15ea78311c.jpg" alt="Embedded Journey Kit - Zero to Hero" width="100%">

# Embedded Journey Kit - Zero to Hero: Arduino examples

[![Compile examples](https://github.com/SolderedElectronics/Soldered-NULA-Beginner-kit-Arduino-project-examples/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/SolderedElectronics/Soldered-NULA-Beginner-kit-Arduino-project-examples/actions/workflows/compile-examples.yml)

Arduino example sketches for the [Embedded Journey Kit - Zero to Hero](https://solde.red/333371). Every example in
this repository is written to be read as much as run: each one explains what every new function does and why the
circuit behaves the way it does, so you can change things and understand what happened.

## Start from zero, become a hero

NULA means zero in Croatian. This kit is your path from absolute beginner to capable maker. Built around the NULA Mini
ESP32-C6, a breadboard-ready programmable board with Wi-Fi 6 and Bluetooth 5.3, it supports both Arduino and
MicroPython. You'll quickly move from blinking your first LED to building connected IoT devices without feeling lost
or overwhelmed.

The kit bridges two worlds: plug-and-play Qwiic modules (no soldering required) and classic breadboard components for
hands-on circuit building.

- **Qwiic ecosystem:** 16x2 LCD, ultrasonic distance sensor, and SHTC3 temperature/humidity sensor connect easily to
  the NULA Mini
- **Classic components:** Mini breadboard, LEDs, photoresistors, buzzer, shift register (74HC595), resistors, and
  jumper wires for fundamental experimentation

You get step-by-step documentation and tutorials for both Arduino and MicroPython, plus guided projects that build
practical skills:

- Blinking LEDs and button interactions
- Light sensing and LCD text displays
- Distance measurement and Wi-Fi communication
- Smart weather station, mini piano, parking sensor
- Morse code transmitter, alarm clock, traffic light simulator
- And more

Designed for STEM classrooms, workshops, and self-learners, this kit helps you move from theory to creating something
that works. Our tutorials get you started quickly while teaching you why circuits work, not just how to copy them.

Ready to start from zero?

> **Documentation and tutorials are coming soon.** Until then, every sketch in this repository is heavily commented and
> can be followed on its own. The `<link placeholder>` markers in the sketch headers are where the tutorial links will
> go.

## What's in the kit

| Component | Quantity | Notes |
| --- | --- | --- |
| NULA Mini ESP32-C6 | 1 | With male headers, breadboard ready |
| LCD Display 16x2 | 1 | Qwiic |
| [Distance Sensor HC-SR04](https://solde.red/333001) | 1 | Qwiic |
| [Temperature and Humidity Sensor SHTC3](https://solde.red/333032) | 1 | Qwiic |
| Qwiic cable, 10 cm | 5 | |
| Mini breadboard | 1 | |
| Jumper wire set for breadboard | 1 | |
| 10 mm colorful LED diode | 13 | 12 single-colour LEDs and 1 RGB LED |
| 10k photoresistor | 3 | |
| Buzzer | 1 | Passive |
| Shift register IC | 1 | 74HC595 |
| 10k THT resistor | 15 | Voltage divider for the photoresistors, plus spares |
| 330 Ohm resistor | 25 | One in series with every LED, to limit the current |

## Getting started

### 1. Install the board

The NULA Mini is an ESP32-C6 board and is part of the official Espressif Arduino core.

1. In the Arduino IDE, open **File -> Preferences** and add this URL to *Additional boards manager URLs*:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
2. Open **Tools -> Board -> Boards Manager**, search for `esp32` and install **esp32 by Espressif Systems**.
3. Select **Tools -> Board -> ESP32 Arduino -> Soldered NULA Mini ESP32C6**.

### 2. Install the libraries

Open **Tools -> Manage Libraries** and install these three, all by Soldered Electronics:

| Library | Needed by | Header it provides |
| --- | --- | --- |
| **Soldered LCD** | sections 4, 7.1, 7.7 | `LCD-SOLDERED.h` |
| **SHTC3 Soldered Library** | sections 5, 7.1 | `SHTC3-SOLDERED.h` |
| **Soldered Ultrasonic Distance Sensor Arduino library** | sections 3, 7.3 | `Ultrasonic-distance-sensor-easyC-SOLDERED.h` |

> **Watch out for the LCD library.** The Library Manager also lists a separate **16x2 LCD Library**, which covers the
> same display but uses a different header name and a different constructor. These examples use **Soldered LCD**. If
> you see `fatal error: LCD-SOLDERED.h: No such file or directory`, you have the other one installed.

Sections 1, 2 and 6 need no libraries at all. Everything they use comes with the board.

### 3. Open an example

Each example lives in its own folder and can be opened directly with **File -> Open**. Start at `1.1_Hello_World` and
work down in order: the examples build on each other, and the later ones point back at the earlier ones instead of
explaining the same thing twice.

## The examples

There are 22 examples across seven sections. Sections 1 to 6 each teach one new thing at a time. Section 7 combines
what you have learned into finished projects.

### 1. Basic skills

| Example | What you learn | Hardware |
| --- | --- | --- |
| [1.1 Hello World](1_Basic_Skills/1.1_Hello_World) | Serial communication, printing to the Serial Monitor | Board only |
| [1.2 LED blinking](1_Basic_Skills/1.2_LED_blinking) | `pinMode()`, `digitalWrite()`, `delay()` | LED, 330 Ohm resistor |

### 2. Inputs and outputs

| Example | What you learn | Hardware |
| --- | --- | --- |
| [2.1 Button counter](2_Inputs_and_Outputs/2.1_Button_Counter) | `digitalRead()`, reading a button, why a raw reading is noisy | Button |
| [2.2 Button debounce](2_Inputs_and_Outputs/2.2_Button_Debounce) | `millis()`, debouncing, toggling a state | Button, LED, 330 Ohm resistor |
| [2.3 Photoresistor analog read](2_Inputs_and_Outputs/2.3_Photoresistor_Analog_Read) | `analogRead()`, the 12-bit ADC, reading a range instead of on/off | Photoresistor, 10k resistor |
| [2.4 Buzzer beep](2_Inputs_and_Outputs/2.4_Buzzer_Beep) | `tone()`, arrays, `for` loops, note frequencies and durations | Buzzer |

### 3. Ultrasonic distance sensor

| Example | What you learn | Hardware |
| --- | --- | --- |
| [3.1 Measuring distance](3_Ultrasonic_Distance_Sensor/3.1_Measuring_Distance) | Using a library over Qwiic, sensor objects, `begin()` then `takeMeasure()` | Ultrasonic sensor, Qwiic cable |
| [3.2 Distance fade LED](3_Ultrasonic_Distance_Sensor/3.2_Distance_Fade_LED) | `map()`, `analogWrite()`, PWM, clamping a value to a range | Ultrasonic sensor, LED, 330 Ohm resistor |

### 4. LCD display

| Example | What you learn | Hardware |
| --- | --- | --- |
| [4.1 Print message](4_LCD_Display/4.1_Print_Message) | `lcd.begin()`, `setCursor()`, `print()`, rows and columns | 16x2 LCD |
| [4.2 Auto scroll text](4_LCD_Display/4.2_Auto_Scroll_Text) | `scrollDisplayLeft()`, animating text with a delay | 16x2 LCD |

### 5. Temperature sensor SHTC3

| Example | What you learn | Hardware |
| --- | --- | --- |
| [5.1 Reading temperature and humidity](5_Temperature_Sensor_SHTC3/5.1_Reading_Temperature_and_Humidity) | I2C over Qwiic, `sample()` before reading, timing with `millis()` | SHTC3 sensor |

### 6. Wi-Fi

| Example | What you learn | Hardware |
| --- | --- | --- |
| [6.1 Connecting and getting data](6_Wi-Fi/6.1_Connecting_and_Getting_Data) | Joining a network, HTTP GET, reading data from a website | Board only |
| [6.2 Wi-Fi LED control](6_Wi-Fi/6.2_Wi-Fi_LED_Control) | Running a web server, routes and handlers, mDNS, a little HTML | LED, 330 Ohm resistor |
| [6.3 Sending data](6_Wi-Fi/6.3_Sending_Data) | HTTP POST, headers, sending your own data to a server | Board only |

Sections 6.1, 6.3 and the Wi-Fi projects need your network name and password filled in at the top of the sketch. The
board connects to 2.4 GHz networks.

### 7. Projects

| Project | What it combines | Hardware |
| --- | --- | --- |
| [7.1 Smart weather station](7_Projects/7.1_Smart_Weather_Station) | SHTC3 + LCD + Wi-Fi, sending readings to a webhook | SHTC3, LCD |
| [7.2 Mini piano](7_Projects/7.2_Mini_piano) | Four buttons mapped to note frequencies | 4 buttons, buzzer |
| [7.3 Parking sensor](7_Projects/7.3_Parking_sensor) | Distance driving beep rate, non-blocking beeping | Ultrasonic sensor, buzzer, LED, 330 Ohm resistor |
| [7.4 RGB LED controller](7_Projects/7.4_RGB_LED_Controller) | One analog input driving three PWM outputs, colour mixing | Photoresistor, RGB LED, 10k resistor, 3x 330 Ohm resistor |
| [7.5 Shift register](7_Projects/7.5_Shift_Register) | `shiftOut()`, latching, binary counting, 8 outputs from 3 pins | 74HC595, 4 LEDs, 4x 330 Ohm resistor |
| [7.6 Morse code transmitter](7_Projects/7.6_Morse_code_transmitter) | Reading text from Serial, lookup tables, structs, your own functions | LED, 330 Ohm resistor |
| [7.7 Alarm clock](7_Projects/7.7_Alarm_Clock) | NTP time over Wi-Fi, LCD, two debounced buttons, buzzer | LCD, 2 buttons, buzzer |
| [7.8 LED traffic light](7_Projects/7.8_LED_Traffic_Light) | Finite state machines, timed sequences without `delay()` | 3 LEDs, 3x 330 Ohm resistor |

## Pins used by each example

Every example is standalone and picks whichever pins suit it, so you will rewire between them. Use this table as a
quick reference. All numbers are the `IO` numbers printed on the board.

| Example | IO2 | IO3 | IO4 | IO5 | IO18 | IO19 |
| --- | --- | --- | --- | --- | --- | --- |
| 1.2 LED blinking | | | LED | | | |
| 2.1 Button counter | | | | | | Button |
| 2.2 Button debounce | | | LED | | | Button |
| 2.3 Photoresistor | | | | Photoresistor | | |
| 2.4 Buzzer beep | | | | Buzzer | | |
| 3.1 Measuring distance | | | | | | |
| 3.2 Distance fade LED | LED | | | | | |
| 6.2 Wi-Fi LED control | | | LED | | | |
| 7.2 Mini piano | Button 1 | Button 2 | Button 3 | Button 4 | Buzzer | |
| 7.3 Parking sensor | Buzzer | | | LED | | |
| 7.4 RGB LED controller | Red | Green | Blue | Photoresistor | | |
| 7.5 Shift register | Data | Latch | Clock | | | |
| 7.6 Morse code | LED | | | | | |
| 7.7 Alarm clock | Hour button | Minute button | Buzzer | | | |
| 7.8 Traffic light | Red | Orange | Green | | | |

The three Qwiic modules - the LCD, the ultrasonic distance sensor and the SHTC3 - do not appear in the table because
they all share the same I2C bus, which on the NULA Mini is **IO6 (SDA)** and **IO7 (SCL)**. Chain them together with the
Qwiic cables and no pin configuration is needed, which is why example 3.1 claims no GPIO pins at all.

Buttons use the **internal pull-up resistors** of the board, switched on with `INPUT_PULLUP` in Arduino and
`Pin.PULL_UP` in MicroPython. That means no resistor is needed on the breadboard: wire one side of the button to the
pin and the other straight to **GND**. It also means the readings are the other way around from what you might expect,
which is called active low: a pin reads **HIGH while its button is released** and **LOW while it is pressed**.

Every LED needs its **own 330 Ohm resistor** in series with it, which limits the current through it. Without one the
LED draws more current than either it or the pin is built for, and both can be damaged. The RGB LED counts as three
LEDs here, so it takes three resistors, one per colour channel.

The photoresistor examples need a **10k resistor** as well. A photoresistor changes its resistance with light but the
board can only measure a voltage, and pairing it with a fixed resistor turns that changing resistance into a changing
voltage. This arrangement is called a voltage divider.


Every example is compiled for the NULA Mini on every push by the
[Compile examples](.github/workflows/compile-examples.yml) workflow, so anything on the main branch is known to build.

## MicroPython

This repository holds the Arduino examples. The same 22 examples exist for MicroPython, with matching numbering and
matching pins, in the
[MicroPython examples repository](https://github.com/SolderedElectronics/Soldered-NULA-Beginner-kit-MicroPython-project-examples).

## Need help?

- Product page: [solde.red/333371](https://solde.red/333371)
- Contact Soldered: [soldered.com/contact](https://soldered.com/contact)
- Found a mistake in an example? Open an issue or a pull request, both are welcome.
