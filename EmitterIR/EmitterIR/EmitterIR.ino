/*
 ___   ______      _______  __   __  ___   _______  _______  _______  ______
|   | |    _ |    |       ||  |_|  ||   | |       ||       ||       ||    _ |
|   | |   | ||    |    ___||       ||   | |_     _||_     _||    ___||   | ||
|   | |   |_||_   |   |___ |       ||   |   |   |    |   |  |   |___ |   |_||_
|   | |    __  |  |    ___||       ||   |   |   |    |   |  |    ___||    __  |
|   | |   |  | |  |   |___ | ||_|| ||   |   |   |    |   |  |   |___ |   |  | |
|___| |___|  |_|  |_______||_|   |_||___|   |___|    |___|  |_______||___|  |_|

																		By Jocelyn Routin

This code show how you can send and receive a simple infrared signal between two Arduino cards.
You need the IRremote library, available here: https://github.com/Arduino-IRremote/Arduino-IRremote
*/

// Include libraries
#include <IRremote.h>

// Define input and output pins
#define FEEDBACK_LED	2
#define IR_LED			3
#define ON_BUTTON		4
#define OFF_BUTTON		5
// Define frames
#define ON_FRAME		0b00000001
#define OFF_FRAME		0b00000000

// Define state graph
typedef enum {
	READY,
	SEND_ON,
	SEND_OFF
} State;

// Declare variables
int buttonOnState = LOW;
int buttonOffState = LOW;
State currentState = READY;
State oldState;

// Setup function: executed once
void setup() {
	// Initialize serial connection
	Serial.begin(9600);
	// Initialize infrared LED connection
	IrSender.begin(IR_LED, false);
	// Initialize other components
	pinMode(FEEDBACK_LED, OUTPUT);
	pinMode(ON_BUTTON, INPUT);
	pinMode(OFF_BUTTON, INPUT);
}

// Loop function: executed in loop
void loop() {
	// Read buttons state
	buttonOnState = digitalRead(ON_BUTTON);
	buttonOffState = digitalRead(OFF_BUTTON);

	// Change state in the graph
	switch (currentState) {
	case READY:
		// If we push the ON button, we send the ON frame
		if (buttonOnState == HIGH) {
			currentState = SEND_ON;
		}
		// If we push the OFF button, we send the OFF frame
		else if (buttonOffState == HIGH) {
			currentState = SEND_OFF;
		}
		break;
	case SEND_ON:
		currentState = READY;
		break;
	case SEND_OFF:
		currentState = READY;
		break;
	}

	// If state changed run actions
	if (currentState != oldState) {
		// Old state becomes current state
		oldState = currentState;
		switch (currentState) {
		case SEND_ON:
			// ON frame sent: we set the feedback LED to HIGH
			digitalWrite(FEEDBACK_LED, HIGH);
			IrSender.sendBoseWave(ON_FRAME, 4);
			Serial.print("Sending frame to receiver: ");
			Serial.println(ON_FRAME, BIN);
			break;
		case SEND_OFF:
			// OFF frame sent: we set the feedback LED to LOW
			digitalWrite(FEEDBACK_LED, LOW);
			IrSender.sendBoseWave(OFF_FRAME, 4);
			Serial.print("Sending frame to receiver: ");
			Serial.println(OFF_FRAME, BIN);
			break;
		}
	}
}