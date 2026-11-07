# Journal

# Entry 1: Project Scope & Architecture
- Selected the ESP32-S3 due to its native USB OTG peripheral, enabling driverless USB HID emulation for media keys and shortcuts.
- Outlined the physical interface: 4 mechanical switches with hot-swap sockets, 1 EC11 rotary encoder for volume control, and an I2C 0.96" OLED screen.
- Finalized the Bill of Materials and mapped out GPIO pins for the schematic.


# Entry 2: Native USB HID Firmware
- Implemented firmware using ESP32-S3 Arduino core with native `USBHIDKeyboard` and `USBHIDConsumerControl`.
- Configured 4 active-low debounced inputs with internal pull-ups for mechanical switches:
  - SW1 (GPIO 4): Terminal launcher.
  - SW2 (GPIO 5): Global audio/mic toggle hotkey.
  - SW3 (GPIO 6): Windows snipping shortcut.
  - SW4 (GPIO 7): Workspace slide navigation.
- Configured EC11 quadrature encoder state-change logic on GPIO 10 & 11 for hardware volume control, with push-to-toggle play/pause on GPIO 12.
- Integrated SSD1306 OLED screen over I2C (SDA: GPIO 8, SCL: GPIO 9) for live status notifications.
