#include <Arduino.h>
#include "states.h"
#include "lcd_manager.h"
#include "servo_manager.h"
#include "rfid_manager.h"

// --- THE MASTER PIN MAP ---
constexpr uint8_t RFID_SS    = D8;
constexpr uint8_t RFID_RST   = D3;
constexpr uint8_t LCD_SCL    = D1;
constexpr uint8_t LCD_SDA    = D2;
constexpr uint8_t SERVO_PIN  = D4;
constexpr uint8_t LED_RED    = D0;
constexpr uint8_t LED_GREEN  = 3;  // RX Pin (GPIO3)
constexpr uint8_t BUZZER_PIN = 1;  // TX Pin (GPIO1)

// --- AUTHORIZED ADMIN CARD ---
const String ADMIN_CARD = "73DD7606"; // YOUR CARD!

LCDManager lcd(0x27, 16, 2);
ServoManager servo;
RFIDManager rfid(RFID_SS, RFID_RST);

SystemState currentState = SystemState::IDLE;
unsigned long stateStartTime = 0;
int failedAttempts = 0;

void setup() {
    // Serial is disabled to allow using RX/TX pins for hardware
    lcd.begin();
    servo.begin(SERVO_PIN); 
    rfid.begin();

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Initial State: Locked
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH); 
    lcd.displayWelcome();
}

void loop() {
    unsigned long currentMillis = millis();

    switch (currentState) {
        case SystemState::IDLE:
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(BUZZER_PIN, LOW);

            if (rfid.isCardPresent()) {
                String scannedUID = rfid.getUID();
                
                if (scannedUID == ADMIN_CARD) {
                    currentState = SystemState::AUTHENTICATING;
                } else {
                    failedAttempts++;
                    currentState = SystemState::ACCESS_DENIED;
                }
                lcd.displayStatus(currentState);
                stateStartTime = currentMillis;
                rfid.halt();
            }
            break;

        case SystemState::AUTHENTICATING:
            if (currentMillis - stateStartTime >= 1000) {
                failedAttempts = 0; 
                currentState = SystemState::ACCESS_GRANTED;
                lcd.displayUser("Esha Srinika", "Admin - Granted");
                
                digitalWrite(LED_RED, LOW);
                digitalWrite(LED_GREEN, HIGH);
                servo.unlock();
                stateStartTime = currentMillis;
            }
            break;

        case SystemState::ACCESS_GRANTED:
            if (currentMillis - stateStartTime >= 5000) { // Keep open for 5 seconds
                servo.lock();
                digitalWrite(LED_GREEN, LOW);
                digitalWrite(LED_RED, HIGH);
                currentState = SystemState::IDLE;
                lcd.displayWelcome();
            }
            break;

        case SystemState::ACCESS_DENIED:
            digitalWrite(BUZZER_PIN, (currentMillis / 200) % 2);
            if (currentMillis - stateStartTime >= 2000) {
                digitalWrite(BUZZER_PIN, LOW);
                if (failedAttempts >= 3) {
                    currentState = SystemState::LOCKED_OUT;
                    lcd.displayStatus(currentState);
                } else {
                    currentState = SystemState::IDLE;
                    lcd.displayWelcome();
                }
                stateStartTime = currentMillis;
            }
            break;

        case SystemState::LOCKED_OUT: {
            unsigned long elapsed = (currentMillis - stateStartTime) / 1000;
            int remaining = 30 - (int)elapsed;
            if (remaining > 0) {
                digitalWrite(LED_RED, (currentMillis / 250) % 2);
                digitalWrite(BUZZER_PIN, (currentMillis / 500) % 2);
                static int lastSec = -1;
                if (remaining != lastSec) {
                    lcd.displayLockout(remaining);
                    lastSec = remaining;
                }
            } else {
                digitalWrite(BUZZER_PIN, LOW);
                failedAttempts = 0;
                currentState = SystemState::IDLE;
                lcd.displayWelcome();
            }
            break;
        }
        default: break;
    }
}
