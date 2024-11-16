### Updated Bill of Materials (BOM)

1. **ESP32 Development Board** (e.g., ESP32-WROOM-32): 1 unit
2. **SSD1306 128x64 OLED Display** (I2C interface): 1 unit
3. **Push Buttons**: 2 units
   - 1 for adjusting trigger level
   - 1 for adjusting sampling rate
4. **Rotary Encoder**: 1 unit
5. **Resistors**:
   - **10kΩ** (for voltage dividers): 8 units
   - **68kΩ, 47kΩ, 22kΩ** (for voltage divider scaling for different modes): 1 unit each
6. **Zener Diodes** (3.3V Zener for input protection): 2 units
7. **Operational Amplifier (Op-Amp)** (e.g., LM358): 1 unit
8. **Analog Multiplexer** (e.g., CD4051 or 74HC4051): 1 unit
9. **Breadboard and Jumper Wires**: As required
10. **Power Supply**: 5V USB power source (or a battery pack)
11. **Decoupling Capacitors** (0.1 µF for noise filtering): 2 units
12. **Ceramic Capacitors** (e.g., 100nF for stability): 2 units

### Complete Schematic & Pinout Routing

#### ESP32 Pin Connections
- **SSD1306 OLED Display (128x64, I2C)**
  - **SDA** (Data): Connect to GPIO 21
  - **SCL** (Clock): Connect to GPIO 22
  - **VCC**: Connect to 3.3V
  - **GND**: Connect to GND

- **Analog Input Signal Conditioning**
  - Use **Voltage Divider** with different resistor values to scale down input voltage for different modes.
  - Connect the input signal to the **CD4051 Analog Multiplexer** inputs.
  - Use an **Op-Amp (LM358)** to add a DC offset to the selected output from the multiplexer to ensure that the output is within **0-3.3V**.
  - Connect the output of the op-amp to **GPIO 34 (ADC_PIN)**.

- **Analog Multiplexer (CD4051)**
  - **Channel Inputs**: Connect each channel to a different voltage divider, each corresponding to a different input voltage range:
    - **Channel 0**: Voltage divider for **-12V to +12V**
    - **Channel 1**: Voltage divider for **-9V to +9V**
    - **Channel 2**: Voltage divider for **-5V to +5V**
    - **Channel 3**: Voltage divider for **-3.3V to +3.3V**
  - **Select Pins**:
    - **A**: Connect to **GPIO 15**
    - **B**: Connect to **GPIO 16**
    - **C**: Connect to **GPIO 17**
  - **Common Output**: Connect to the **Op-Amp** input

- **Zener Protection**:
  - **Zener Diode (3.3V)** between the ADC input pin and GND to clamp any overvoltage.
  - A **series resistor** (1kΩ) before the ADC input.

- **Push Buttons**:
  - **Button 1** (Trigger Level Adjustment): Connect one side to **GPIO 12** and the other side to GND.
  - **Button 2** (Sample Rate Adjustment): Connect one side to **GPIO 13** and the other side to GND.

- **Rotary Encoder**:
  - **Encoder Pin A**: Connect to **GPIO 25**
  - **Encoder Pin B**: Connect to **GPIO 26**
  - **VCC**: Connect to 3.3V
  - **GND**: Connect to GND

#### Power Supply
- **ESP32 Power Input**: Connect via USB to a 5V power supply or battery pack.

### Schematic Diagram Description
- The **ESP32** board serves as the core controller, reading the analog input and driving the OLED display.
- The **input signal** is first routed through the **CD4051 analog multiplexer**, which selects the appropriate **voltage divider** based on the measured signal range.
- Each channel of the multiplexer is connected to a different **voltage divider**, allowing the signal to be scaled down appropriately for the selected mode.
- The **op-amp** adds a DC offset to the selected output from the multiplexer, ensuring the resulting signal is within the ADC range (**0-3.3V**).
- **Push Buttons** are used to adjust the trigger level and sample rate.
- The **rotary encoder** allows fine control over the voltage scaling, with **GPIO 25** and **GPIO 26** used as inputs.
- **Zener Diodes** are used to protect the ADC input from overvoltage, ensuring the input does not exceed **3.3V**.
- The **ESP32** automatically determines the input mode by measuring the peak-to-peak voltage of the input signal and adjusts the multiplexer channel accordingly.

### Guide for Assembly
1. **ESP32 Placement**:
   - Place the ESP32 board on a breadboard.
   - Connect **GND** and **3.3V** lines across the breadboard for other components.

2. **OLED Display Wiring**:
   - Connect **VCC** to **3.3V**, **GND** to **GND**.
   - **SDA** to **GPIO 21**, **SCL** to **GPIO 22**.

3. **Analog Signal Conditioning**:
   - Connect the input signal to the **CD4051 multiplexer**.
   - Each channel of the multiplexer is connected to a different **voltage divider** (configured for different input ranges).
   - The common output from the multiplexer is fed into an **op-amp** for adding a DC offset.
   - The output of the op-amp is connected to **GPIO 34 (ADC_PIN)** of the ESP32.

4. **Push Button Wiring**:
   - Connect each button between the corresponding GPIO pin and **GND**.

5. **Rotary Encoder Wiring**:
   - Connect **Pin A** to **GPIO 25** and **Pin B** to **GPIO 26**.
   - Connect **VCC** to **3.3V** and **GND** to **GND**.

6. **Multiplexer Control**:
   - Connect **MUX_PIN_A** to **GPIO 15**.
   - Connect **MUX_PIN_B** to **GPIO 16**.
   - Connect **MUX_PIN_C** to **GPIO 17**.
   - These pins control which input channel of the **CD4051** is active, and are set automatically by the ESP32 based on the measured input signal.

7. **Zener Protection Circuit**:
   - Use a **3.3V Zener diode** between the input of **GPIO 34** and **GND**.
   - Use a **series resistor** (e.g., **1kΩ**) before the ADC input to limit the current.

8. **Power Supply**:
   - Power the ESP32 through its USB port.

### Notes
- **Automatic Input Mode Selection**: The ESP32 automatically selects the input mode by measuring the incoming signal's peak-to-peak voltage and adjusting the multiplexer channel to match the appropriate voltage range (**-12V to +12V, -9V to +9V, -5V to +5V, -3.3V to +3.3V**).
- **Trigger Slope Detection**: The firmware now includes trigger slope detection, allowing the oscilloscope to trigger on either rising or falling edges, ensuring accurate signal capture.
- **Voltage Divider and Offset**: The voltage divider reduces the input to safe levels, and the op-amp offsets it to fit within **0-3.3V** for the ESP32 ADC.
- **Multiplexer Control**: The **CD4051 multiplexer** allows selecting the appropriate voltage divider for the input range, controlled automatically by the ESP32 based on the measured signal.
- **User Interface**: The OLED display provides visualization of the waveform along with information about the current settings, such as sample rate and trigger level.
- **Optional**: Use **decoupling capacitors** near the power pins of the ESP32 and other components to reduce noise.

