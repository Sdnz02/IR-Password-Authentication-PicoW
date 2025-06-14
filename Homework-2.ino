#include <IRremote.hpp>

#define IR_PIN 2 // Pin connected to the IR receiver module
#define RED_LED_PIN 10 // Pins connected to the RGB LED
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define BUZZER_PIN 9 // Pin connected to the buzzer

// Define the correct password sequence (9 characters)
const unsigned long PASSWORD[] = {0xBA45FF00, 0xB946FF00, 0xB847FF00, 0xBB44FF00, 0xBF40FF00, 0xBC43FF00, 0xF807FF00, 0xEA15FF00, 0xF609FF00};

// Initialize variables for password input
unsigned long userInput[9];
int inputIndex = 0;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN); // Start the IR receiver

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long value = IrReceiver.decodedIRData.decodedRawData; // Get the decoded value

    // Check if the button press matches the expected password
    if (value == PASSWORD[inputIndex]) {
      // Store the input and move to the next index
      userInput[inputIndex] = value;
      inputIndex++;

      // If the entire password has been entered
      if (inputIndex == sizeof(PASSWORD) / sizeof(PASSWORD[0])) {
        successAction(7); // Pass the ID number to the success action function
        inputIndex = 0; // Reset input index
      }

    }
    else {
      // If the button press does not match the expected password
      failureAction(); // Perform failure action
      inputIndex = 0; // Reset input index
    }
    Serial.println(value, HEX);
    IrReceiver.resume(); // Receive the next value
  }
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_LED_PIN, red);
  analogWrite(GREEN_LED_PIN, green);
  analogWrite(BLUE_LED_PIN, blue);
}

void successAction(int id) {
  // Display the ID number (example: blink the LED multiple times)
  for (int i = 0; i < id; i++) {
    setColor(0, 255, 0); // Set LED to green
    delay(500); // Delay to indicate success
    setColor(0, 0, 0); // Turn off LED
    delay(500); // Delay
  }
}
void failureAction() {
  setColor(255, 0, 0); // Set LED to red
  tone(BUZZER_PIN, 1000, 1000); // Continuous beep to indicate failure
  delay(2000); // Delay to indicate failure
  noTone(BUZZER_PIN); // Turn off buzzer
  setColor(0, 0, 0); // Turn off LED after failure
}
