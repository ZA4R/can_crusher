#define START_B 4
#define SHUTDOWN_B 3
#define EXTEND_PIN 5 // right PWM on BTS7960 if wired correctly
#define RETRACT_PIN 6
#define IR_TRANSMITTER_PIN 9
#define IR_RECEIVER_PIN 8

unsigned long timeStart;
const long interval = 15000; // 15 sec timer for can crushing

volatile bool lock = false;


void setup() {
  pinMode(START_B, INPUT_PULLUP);
  pinMode(SHUTDOWN_B, INPUT_PULLUP);
  pinMode(EXTEND_PIN, OUTPUT);
  pinMode(RETRACT_PIN, OUTPUT);
  pinMode(IR_RECEIVER_PIN, INPUT_PULLUP);
  pinMode(IR_TRANSMITTER_PIN, OUTPUT);

  digitalWrite(EXTEND_PIN, LOW);
  digitalWrite(RETRACT_PIN, LOW);

  tone(IR_TRANSMITTER_PIN, 38000);

  interrupts();
  attachInterrupt(digitalPinToInterrupt(SHUTDOWN_B), shutdown, FALLING);

  resetCrusher();
  
  Serial.begin(9600);
  delay(500);
}

/*
 * if start button is pressed when lock is deactive then can crushing will commence
 * else if the start button is pressed when lock is active then it will reset the crusher and unlock
 */
void loop() {
  if (lock) {
    if (digitalRead(START_B) == LOW) {
      lock = false;
      resetCrusher();
    }
  } else {
    // if start button is pressed or object is detected then crush can
    if (digitalRead(START_B) == LOW || digitalRead(IR_RECEIVER_PIN) == LOW) {
      crushCan();
    }
  }
  delay(500);
}

// interrupt intended to be used as a safety mechanism
void shutdown() {
    lock = true;
}

void crushCan() {
  delay(20); // prevent H-bridge short circuit
  digitalWrite(EXTEND_PIN, HIGH);
  timeStart = millis();
  while (millis() - timeStart < interval) {
    if (lock) {
      break; // exit program and return to main loop which should now be inactive due to lock
    }
  }
  noInterrupts(); // attempt to stop interrupt from triggering unintentionally
  digitalWrite(EXTEND_PIN, LOW);
  if (!lock) {
    resetCrusher();
    return;
  }
  interrupts();
}

void resetCrusher() {
  delay(20); // prevent H-bridge short circuit
  interrupts();
  digitalWrite(RETRACT_PIN, HIGH);
  timeStart = millis();
  while (millis() - timeStart < interval) {
    if (lock) {
      break; // exit program and return to main loop which should now be inactive due to lock
    }
  }
  noInterrupts();
  digitalWrite(RETRACT_PIN, LOW);
  delay(20);
  interrupts();
}
