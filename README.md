Arduino Project with DHT11, LDR, LCD, and Stepper Motor Control
Overview
This project integrates a DHT11 sensor for measuring temperature and humidity, an LDR for light detection, an LCD for displaying readings, and a stepper motor for mechanical movement. The project aims to read environmental data and control an LED and motors based on these readings.

Components
Arduino board
DHT11 sensor
LDR (Light Dependent Resistor)
LCD display (I2C interface)
Stepper motors with TP6600 driver
LEDs
Pin Configuration
LEDs:
ledPin (pin 2): LED 1
ledPin3 (pin 3): LED 2
Sensors:
analogPin (A0): LDR
dht (pin 3): DHT11 sensor
Stepper Motors:
X-axis:
X_STEP_PIN (pin 9)
X_DIR_PIN (pin 10)
X_EN_PIN (pin 8)
Y-axis:
Y_STEP_PIN (pin 5)
Y_DIR_PIN (pin 7)
Y_EN_PIN (pin 6)
LCD Display:
I2C interface with default address 0x27
