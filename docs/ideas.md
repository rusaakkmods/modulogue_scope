### Project Overview: ESP32 OLED Oscilloscope

#### Objective
The goal of this project is to create a simple, portable oscilloscope using an ESP32 microcontroller and an SSD1306 128x64 OLED display. The oscilloscope will be able to capture and display analog signals, with adjustable trigger levels, sample rates, and voltage scaling. The project is designed to be cost-effective and user-friendly, providing essential waveform visualization for educational and experimental purposes.

#### Features
1. **Signal Capture**:
   - Analog input via the ESP32's ADC to capture signals in the range of 0-3.3V.
   - Adjustable trigger level to stabilize waveform display.
   - Adjustable sample rate to control time base.

2. **User Interface**:
   - **OLED Display**: 128x64 pixel resolution for waveform visualization.
   - **Push Buttons**: Four buttons for adjusting trigger level and sample rate, including coarse and fine adjustments.
   - **Rotary Encoder**: Allows finer control over voltage scaling, and can be used for navigating potential future menu options.

3. **Power Supply**:
   - Powered via USB or a 5V regulated power supply.

4. **Compact Design**:
   - All components can be mounted on a breadboard for prototyping, making it portable and easy to assemble.

#### Key Components
- **ESP32 Development Board**: Handles signal processing, ADC capture, and interfacing with the display and controls.
- **SSD1306 128x64 OLED Display**: Provides a compact interface for visualizing captured waveforms.
- **Push Buttons**: Four buttons are used for user control over trigger level and sample rate, offering both coarse and fine adjustments.
- **Rotary Encoder**: Used for finer voltage scaling adjustments, with the possibility of expanding its functionality for menu navigation.

#### Functional Description
- The **ESP32** reads analog input from **GPIO 34** to capture real-time signals.
- The **OLED display** shows the waveform, with adjustable settings that allow the user to modify how the waveform is captured and displayed.
- **Trigger functionality** allows stable visualization of repetitive signals by starting the display at a set voltage level.
- **Sample rate** can be adjusted to zoom in or out on the waveform, making it possible to view more or fewer signal cycles on the screen.
- The **rotary encoder** provides fine-grained control over the vertical scale of the waveform, allowing the user to zoom in or out vertically.

#### Applications
- **Educational Use**: Suitable for students learning about signal processing, electronics, and microcontrollers.
- **Hobbyist Projects**: Ideal for electronics enthusiasts who want a low-cost tool for viewing analog signals in their projects.
- **Basic Signal Debugging**: Can be used to observe and debug basic analog signals in small-scale electronic projects.

#### Future Improvements
- **Dual Channel Input**: Adding a second analog input for capturing two signals simultaneously.
- **FFT Analysis**: Implementing frequency analysis to provide additional insight into the captured signals.
- **Wi-Fi/Bluetooth Connectivity**: Streaming data to a web or mobile interface for remote viewing.
- **Data Logging**: Adding an SD card module for data logging and later analysis of waveforms.

This project aims to offer an accessible introduction to oscilloscopes and waveform analysis, providing just enough functionality for educational and practical use without the complexity and cost of a commercial oscilloscope.