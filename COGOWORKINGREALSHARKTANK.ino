#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <math.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin definitions
const int buttonPin = 2;
const int loudPin = A1; 
const int pingPin = 7;
const int laserPin = 13;
const int LED_PIN = 9;
const int RED_LED_PIN = 10; 
const int LDR_PIN = A0; 
const int sensorPin = A5; 

// Constants
const int NUM_READINGS = 10; // Updated to use 10 readings for averaging
const int LIGHT_THRESHOLD_LOW = 79;
const int LIGHT_THRESHOLD_HIGH = 91;
const int UpperThreshold = 560;
const int LowerThreshold = 530;
const unsigned long dateDisplayInterval = 5000; // 5 seconds
const unsigned long sensorDisplayInterval = 10000; // 10 seconds
const unsigned long pulseDisplayInterval = 30000; // 30 seconds

// Global variables
int ldrReadings[NUM_READINGS];
int readingIndex = 0;
int total = 0;
int averageLDR = 0;
int loudness;
int lastLoudness = 0;
long distanceInches = 0;
bool distanceScreenActive = true; // Variable to track distance screen state

// Button debounce variables
const int debounceDelay = 50; // Debounce time in milliseconds
unsigned long lastDebounceTime = 0;
bool lastButtonState = HIGH;
bool buttonState = HIGH;
bool toggled = false;

// Timing variables
unsigned long previousMillis = 0;
unsigned long screenStartMillis = 0;
enum ScreenState { DATE, SENSOR, PULSE };
ScreenState currentScreen = DATE;

// Scrolling variables for pulse graph
int scrollPositionX = 0; // Horizontal scroll position
const int SCROLL_SPEED_X = 2; // Increased speed for faster scrolling

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(loudPin, INPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT); // Initialize red LED pin
  pinMode(LDR_PIN, INPUT);
  pinMode(sensorPin, INPUT); // Initialize pulse sensor pin

  // Initialize LDR readings array
  for (int i = 0; i < NUM_READINGS; i++) {
    ldrReadings[i] = 0;
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Infinite loop if display initialization fails
  }

  // Initialize internal time
  setTime(10, 0, 0, 2, 8, 2024); // Set initial time

  digitalWrite(laserPin, HIGH); // Ensure laser starts off
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle button press with debounce
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW && !toggled) {
        distanceScreenActive = !distanceScreenActive;
        screenStartMillis = currentMillis;
        toggled = true;
      }
    }
  }

  if ((currentMillis - lastDebounceTime) > debounceDelay * 2) {
    toggled = false;
  }

  lastButtonState = reading;

  if (distanceScreenActive) {
    readDistanceSensor();
    digitalWrite(laserPin, HIGH);
    displayDistanceSensor();
    Serial.print("Distance: ");
    Serial.print(distanceInches);
    Serial.println(" in");
    return;
  } else {
    digitalWrite(laserPin, LOW);
  }

  // Swap screens based on intervals
  unsigned long interval = (currentScreen == DATE) ? dateDisplayInterval :
                           (currentScreen == SENSOR) ? sensorDisplayInterval :
                           pulseDisplayInterval;

  if (currentMillis - screenStartMillis >= interval) {
    screenStartMillis = currentMillis;

    if (currentScreen == DATE) {
      currentScreen = SENSOR;
    } else if (currentScreen == SENSOR) {
      currentScreen = PULSE;
    } else if (currentScreen == PULSE) {
      currentScreen = DATE;
    }
  }

  // Update loudness value every 1 second if sensor screen is active
  if (currentMillis - previousMillis >= 1000 && currentScreen == SENSOR) {
    previousMillis = currentMillis;
    lastLoudness = analogRead(loudPin);
    Serial.println("Loudness: " + String(lastLoudness));

    // Control the red LED based on loudness
    if (lastLoudness > 60) {
      digitalWrite(RED_LED_PIN, HIGH); // Turn on the red LED
    } else {
      digitalWrite(RED_LED_PIN, LOW);  // Turn off the red LED
    }
  }

  // Display information based on current screen
  if (currentScreen == DATE) {
    displayDateTime();
  } else if (currentScreen == SENSOR) {
    displaySensorInfo();
  } else if (currentScreen == PULSE) {
    displayPulseGraph();
  }

  // LDR logic for controlling LED
  total = total - ldrReadings[readingIndex];
  ldrReadings[readingIndex] = analogRead(LDR_PIN);
  total = total + ldrReadings[readingIndex];
  readingIndex = (readingIndex + 1) % NUM_READINGS;
  averageLDR = total / NUM_READINGS;

  Serial.print("Average LDR Value: ");
  Serial.println(averageLDR);

  if (averageLDR < LIGHT_THRESHOLD_LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else if (averageLDR > LIGHT_THRESHOLD_HIGH) {
    digitalWrite(LED_PIN, LOW);
  }

  delay(100);
}

void readDistanceSensor() {
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  distanceInches = microsecondsToInches(duration);
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

void displayDateTime() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Fetch and display the current time from internal time
  String timeStr = String(hour()) + ":" + String(minute()) + ":" + String(second());
  String dateStr = String(day()) + " " + String(month()) + " " + String(year());

  display.setCursor(20, 5);
  display.println(timeStr);
  display.setCursor(20, 20);
  display.println(dateStr);

  display.display();
}

void displaySensorInfo() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  if (lastLoudness > 60) {
    display.print(lastLoudness);
    display.println(F("-Wear Ear muffs"));
  } else {
    display.print(lastLoudness);
    display.println(F("-Loudness is OK"));
  }

  display.setCursor(0, 16);
  display.print(F("UV Index:1;UV Safe"));

  display.setCursor(0, 32);
  display.print(F("Fresh Air"));

  display.display();
}

void displayDistanceSensor() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 10);
  display.print(F("Distance: "));
  display.print(distanceInches);
  display.println(F(" in"));

  display.display();
}

void displayPulseGraph() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display the title
  display.setCursor(0, 0);
  display.print(F("Pulse Graph"));

  // Define key points for the pulse graph
  int pulsePoints[SCREEN_WIDTH];
  int numPoints = sizeof(pulsePoints) / sizeof(pulsePoints[0]);

  // Simulate pulse wave data
  for (int i = 0; i < numPoints; i++) {
    float phase = (i + scrollPositionX) * 0.1;
    pulsePoints[i] = (int)(20 * (1 - exp(-0.1 * phase)) * (sin(phase) + 1)); // Create pulse waveform
    if (pulsePoints[i] < 0) pulsePoints[i] = 0;
    if (pulsePoints[i] > SCREEN_HEIGHT) pulsePoints[i] = SCREEN_HEIGHT;
  }

  // Draw lines between the points
  for (int i = 1; i < numPoints; i++) {
    display.drawLine(i - 1, SCREEN_HEIGHT - pulsePoints[i - 1], i, SCREEN_HEIGHT - pulsePoints[i], SSD1306_WHITE);
  }

  // Update scroll position
  scrollPositionX += SCROLL_SPEED_X;

  // Reset scroll position to create a continuous effect
  if (scrollPositionX >= numPoints) {
    scrollPositionX = 0;
  }

  display.display();
}
