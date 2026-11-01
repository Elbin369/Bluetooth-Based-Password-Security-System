#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ========== OLED CONFIGURATION ==========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

// ========== HARDWARE PINS ==========
const int GREEN_LED = PB13;
const int RED_LED   = PB12;
const int BUZZER    = PB14;

// ========== PASSWORDS ==========
const char PASSWORD[] = "1234";
const char RECOVERY_PASSWORD[] = "7777";

// ========== SECURITY SETTINGS ==========
const int MAX_ATTEMPTS = 3;
int wrongAttempts = 0;

bool lockedOut = false;
bool unlocked = false;

const unsigned long UNLOCK_TIME = 7000;
unsigned long unlockStartedAt = 0;

// ========== BLUETOOTH INPUT ==========
String receivedPassword = "";
bool inputStarted = false;

// ========== OLED DISPLAY ==========
void showMessage(const char* line1,
                 const char* line2 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 15);
  display.println(line1);
  display.println();
  display.println(line2);
  display.display();
}

// ========== LED BLINKING ==========
void blinkLED(int pin, int times, int intervalMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(intervalMs);
    digitalWrite(pin, LOW);
    delay(intervalMs);
  }
}

// ========== BUZZER PATTERNS ==========
// Assumes an active buzzer with HIGH = ON.

void shortBeep() {
  digitalWrite(BUZZER, HIGH);
  delay(120);
  digitalWrite(BUZZER, LOW);
  delay(100);
}

void longBeep() {
  digitalWrite(BUZZER, HIGH);
  delay(450);
  digitalWrite(BUZZER, LOW);
  delay(150);
}

// Correct password: two short beeps
void successSound() {
  shortBeep();
  shortBeep();
}

// Wrong normal password: one short beep
void errorSound() {
  shortBeep();
}

// Wrong recovery password: two short beeps
void recoveryErrorSound() {
  shortBeep();
  shortBeep();
}

// Automatic relock: one long beep
void relockSound() {
  longBeep();
}

// Full lockout: three short, then two long beeps
void lockoutSound() {
  shortBeep();
  shortBeep();
  shortBeep();

  delay(350);

  longBeep();
  longBeep();
}

// ========== PASSWORD PROMPT ==========
void askForPassword() {
  showMessage("PASSWORD LOCK", "Enter password:");
  Serial1.println("Enter password:");
}

// ========== UNLOCK SYSTEM ==========
void unlockSystem() {
  lockedOut = false;
  unlocked = true;
  wrongAttempts = 0;

  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  unlockStartedAt = millis();

  showMessage("ACCESS UNLOCKED", "Auto-lock: 7 sec");

  Serial1.println("ACCESS UNLOCKED");
  Serial1.println("Auto-lock in 7 seconds");

  successSound();

  // Green LED blinks five times
  blinkLED(GREEN_LED, 5, 250);

  // Green LED remains ON during unlocked state
  digitalWrite(GREEN_LED, HIGH);
}

// ========== FULL LOCKOUT ==========
void enterLockout() {
  lockedOut = true;
  unlocked = false;

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // Do not reveal the recovery password
  showMessage("LOCKED OUT!", "Access restricted");

  Serial1.println("LOCKED OUT!");
  Serial1.println("Access restricted.");

  // Special lockout alarm
  lockoutSound();

  // Red LED warning
  blinkLED(RED_LED, 6, 250);
  digitalWrite(RED_LED, LOW);

  // Keep recovery password secret
  showMessage("LOCKED OUT!", "Access restricted");
}

// ========== PASSWORD PROCESSING ==========
void handlePassword(String entered) {
  entered.trim();

  if (entered.length() == 0) {
    return;
  }

  // Ignore further entries while unlocked
  if (unlocked) {
    Serial1.println("Already unlocked.");
    return;
  }

  // ----- LOCKED-OUT MODE -----
  if (lockedOut) {
    if (entered == RECOVERY_PASSWORD) {
      Serial1.println("Recovery accepted.");
      unlockSystem();
    } else {
      digitalWrite(GREEN_LED, LOW);

      showMessage("LOCKED OUT!", "Invalid code");
      Serial1.println("Invalid code. Still locked.");

      recoveryErrorSound();

      blinkLED(RED_LED, 4, 250);
      digitalWrite(RED_LED, LOW);

      // Never show the recovery password
      showMessage("LOCKED OUT!", "Access restricted");
    }

    return;
  }

  // ----- NORMAL PASSWORD MODE -----
  if (entered == PASSWORD) {
    unlockSystem();
  } else {
    wrongAttempts++;

    digitalWrite(GREEN_LED, LOW);

    showMessage("WRONG PASSWORD!", "Try again");
    Serial1.println("WRONG PASSWORD!");

    errorSound();

    blinkLED(RED_LED, 4, 250);
    digitalWrite(RED_LED, LOW);

    Serial1.print("Attempts: ");
    Serial1.print(wrongAttempts);
    Serial1.print("/");
    Serial1.println(MAX_ATTEMPTS);

    if (wrongAttempts >= MAX_ATTEMPTS) {
      enterLockout();
    } else {
      askForPassword();
    }
  }
}

// ========== SETUP ==========
void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  // OLED I2C: PB6 = SCL, PB7 = SDA
  Wire.setSCL(PB6);
  Wire.setSDA(PB7);
  Wire.begin();

  // HC-05/HC-06: PA9 TX, PA10 RX
  Serial1.begin(9600);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) {
      // Stop if OLED initialization fails
    }
  }

  showMessage("STM32 PASSWORD", "System starting...");
  delay(1000);

  askForPassword();
}

// ========== MAIN LOOP ==========
void loop() {
  // Automatically relock after seven seconds
  if (unlocked &&
      (unsigned long)(millis() - unlockStartedAt)
        >= UNLOCK_TIME) {

    unlocked = false;

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

    showMessage("SYSTEM LOCKED", "Enter password:");
    Serial1.println("AUTO-LOCKED");

    relockSound();

    delay(300);
    askForPassword();
  }

  // Read incoming Bluetooth characters
  while (Serial1.available() > 0) {
    char c = (char)Serial1.read();

    if (c == '\r' || c == '\n') {
      if (inputStarted &&
          receivedPassword.length() > 0) {

        handlePassword(receivedPassword);
      }

      receivedPassword = "";
      inputStarted = false;
    }
    else if (c >= 32 && c <= 126) {
      inputStarted = true;

      if (receivedPassword.length() < 32) {
        receivedPassword += c;
      } else {
        // Reject oversized input
        receivedPassword = "";
        inputStarted = false;
      }
    }
  }
}
