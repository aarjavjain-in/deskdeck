# JOURNAL

# November 7, 2026: Project Scope & Architecture

Selected the ESP32-S3 due to its native USB OTG peripheral, enabling driverless USB HID emulation for media keys and shortcuts. Outlined the physical interface: 4 mechanical switches with hot-swap sockets, 1 EC11 rotary encoder for volume control, and an I2C 0.96" OLED screen. Finalized the Bill of Materials and mapped out GPIO pins.

![Architecture Diagram](https://raw.githubusercontent.com/aarjavjain-in/deskdeck/main/docs/architecture.png)

# November 7, 2026: Firmware Implementation

Implemented firmware using ESP32-S3 Arduino core with native USBHIDKeyboard and USBHIDConsumerControl. Configured active-low inputs with internal pull-ups for mechanical switches and the EC11 quadrature encoder. Integrated SSD1306 OLED screen over I2C.

![Firmware Setup](https://raw.githubusercontent.com/aarjavjain-in/deskdeck/main/docs/firmware.png)
