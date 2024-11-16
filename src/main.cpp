#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <driver/timer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ADC_PIN 34
#define BUTTON_PIN_1 12 // Pin for adjusting trigger level
#define BUTTON_PIN_2 13 // Pin for adjusting sampling rate
#define ENCODER_PIN_A 25 // Pin A for rotary encoder
#define ENCODER_PIN_B 26 // Pin B for rotary encoder
#define MUX_PIN_A 15 // Multiplexer select pin A
#define MUX_PIN_B 16 // Multiplexer select pin B
#define MUX_PIN_C 17 // Multiplexer select pin C

int sampleRate = 10000; // Default 10 kHz sample rate
int triggerLevel = 2048; // Default trigger level (mid-range)
float voltageRatio = 1.0; // Default voltage ratio for -12V to +12V

const int BUFFER_SIZE = 64; // Buffer size for ADC readings
uint16_t adcBuffer[BUFFER_SIZE];
volatile int bufferIndex = 0;

volatile int encoderValue = 0;
volatile bool encoderChanged = false;

void IRAM_ATTR handleEncoder() {
  if (digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)) {
    encoderValue++;
  } else {
    encoderValue--;
  }
  encoderChanged = true;
}

void IRAM_ATTR onTimer() {
  if (bufferIndex < BUFFER_SIZE) {
    adcBuffer[bufferIndex++] = analogRead(ADC_PIN);
  } else {
    bufferIndex = 0;
    timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, TIMER_0, 1000000 / sampleRate);
  }
}

void setup() {
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // ADC setup
  analogReadResolution(12); // Set ADC resolution to 12-bit (0-4095)
  pinMode(ADC_PIN, INPUT);

  // Button setup
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  // Rotary encoder setup
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoder, CHANGE);

  // Multiplexer select pins setup
  pinMode(MUX_PIN_A, OUTPUT);
  pinMode(MUX_PIN_B, OUTPUT);
  pinMode(MUX_PIN_C, OUTPUT);

  // Timer setup for ADC sampling
  timer_config_t config = {
    .alarm_en = TIMER_ALARM_EN,
    .counter_en = TIMER_PAUSE,
    .intr_type = TIMER_INTR_LEVEL,
    .counter_dir = TIMER_COUNT_UP,
    .auto_reload = TIMER_AUTORELOAD_EN,
    .divider = 80 // 1 microsecond per tick
  };
  timer_init(TIMER_GROUP_0, TIMER_0, &config);
  timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1000000 / sampleRate);
  timer_enable_intr(TIMER_GROUP_0, TIMER_0);
  timer_isr_register(TIMER_GROUP_0, TIMER_0, (void (*)(void*))onTimer, NULL, ESP_INTR_FLAG_IRAM, NULL);
}

void setMultiplexerChannel(int channel) {
  // Set the multiplexer channel using the select pins (A, B, C)
  digitalWrite(MUX_PIN_A, channel & 0x01);
  digitalWrite(MUX_PIN_B, (channel >> 1) & 0x01);
  digitalWrite(MUX_PIN_C, (channel >> 2) & 0x01);
}

void loop() {
  // Check button presses to adjust trigger level and sample rate
  if (digitalRead(BUTTON_PIN_1) == LOW) {
    delay(200); // Debounce delay
    triggerLevel += 200; // Increase trigger level
    if (triggerLevel > 4095) {
      triggerLevel = 0; // Wrap around
    }
  }

  if (digitalRead(BUTTON_PIN_2) == LOW) {
    delay(200); // Debounce delay
    sampleRate += 1000; // Increase sample rate
    if (sampleRate > 20000) {
      sampleRate = 1000; // Wrap around to a minimum rate of 1 kHz
    }
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1000000 / sampleRate);
  }

  // Rotary encoder for voltage scaling
  if (encoderChanged) {
    encoderValue = 0;
    encoderChanged = false;
  }

  // Start the timer to begin sampling
  bufferIndex = 0;
  timer_start(TIMER_GROUP_0, TIMER_0);
  timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);

  // Wait for sampling to complete
  while (bufferIndex < BUFFER_SIZE) {
    // Wait
  }

  // Display the waveform on the OLED with scaling and UI elements
  display.clearDisplay();

  // Draw waveform
  for (int i = 1; i < BUFFER_SIZE; i++) {
    int y1 = (int)((adcBuffer[i - 1] * voltageRatio) / 4095.0 * (SCREEN_HEIGHT - 1));
    int y2 = (int)((adcBuffer[i] * voltageRatio) / 4095.0 * (SCREEN_HEIGHT - 1));
    display.drawLine(i - 1, SCREEN_HEIGHT - 1 - y1, i, SCREEN_HEIGHT - 1 - y2, SSD1306_WHITE);
  }

  // Display minimal UI elements
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("TrigLvl: ");
  display.print(triggerLevel * 3.3 / 4095, 2); // Display trigger level in volts (assuming 3.3V ADC)
  display.print("V");

  display.setCursor(0, 10);
  display.print("SRate: ");
  display.print(sampleRate);
  display.print(" Hz");

  display.display();
}
