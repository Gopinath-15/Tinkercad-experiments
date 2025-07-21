// Variable declarations and initializations
const int redLED = 13;
const int yellowLED = 12;
const int greenLED = 8;
const int buttonPin = 2;

int buttonState = 0;
int lastButtonState = HIGH; // For simple edge detection

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  // Start with the Red LED ON
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
}

void loop() {
  // Traffic states: 0 - Red, 1 - Yellow, 2 - Green
  static int state = 0;
  static unsigned long lastChange = millis();
  static bool restartFlag = false;

  // Read the button (active low)
  buttonState = digitalRead(buttonPin);

  // Edge detection for button: trigger only on press, not while held
  if (lastButtonState == HIGH && buttonState == LOW) {
    restartFlag = true;
  }
  lastButtonState = buttonState;

  // If restart flag set, go to RED immediately
  if (restartFlag) {
    state = 0;
    lastChange = millis();
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    Serial.println("RESTART: RED light ON: Please Stop!");
    restartFlag = false;
  }

  // Main state machine
  unsigned long currentMillis = millis();

  switch(state) {
    case 0: // Red
      if (currentMillis - lastChange >= 10000) {
        state = 1;
        lastChange = currentMillis;
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
        Serial.println("YELLOW light ON: Get Ready!");
      }
      break;
    case 1: // Yellow
      if (currentMillis - lastChange >= 10000) {
        state = 2;
        lastChange = currentMillis;
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
        Serial.println("GREEN light ON: Go Now!");
      }
      break;
    case 2: // Green
      if (currentMillis - lastChange >= 10000) {
        state = 0;
        lastChange = currentMillis;
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        Serial.println("RED light ON: Please Stop!");
      }
      break;
  }
}
