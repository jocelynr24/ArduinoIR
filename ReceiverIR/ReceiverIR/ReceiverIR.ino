/*
 ___   ______      ______    _______  _______  _______  ___   __   __  _______  ______
|   | |    _ |    |    _ |  |       ||       ||       ||   | |  | |  ||       ||    _ |
|   | |   | ||    |   | ||  |    ___||       ||    ___||   | |  |_|  ||    ___||   | ||
|   | |   |_||_   |   |_||_ |   |___ |       ||   |___ |   | |       ||   |___ |   |_||_
|   | |    __  |  |    __  ||    ___||      _||    ___||   | |       ||    ___||    __  |
|   | |   |  | |  |   |  | ||   |___ |     |_ |   |___ |   |  |     | |   |___ |   |  | |
|___| |___|  |_|  |___|  |_||_______||_______||_______||___|   |___|  |_______||___|  |_|

																		By Jocelyn Routin

This code show how you can send and receive a simple infrared signal between two Arduino cards.
You need the IRremote library, available here: https://github.com/Arduino-IRremote/Arduino-IRremote
*/

// Include libraries
#include <IRremote.h>

// Define input and output pins
#define LED			2
#define IR_LED		3
// Define frames
#define ON_FRAME	0b00000001
#define OFF_FRAME	0b00000000

// Define state graph
typedef enum {
	LISTEN,
	SWITCH_ON,
	SWITCH_OFF
} State;

// Declare variables
State currentState = LISTEN;
State oldState;

// Setup function: executed once
void setup() {
	// Initialize serial connection
	Serial.begin(9600);
	// Initialize infrared LED connection
	IrReceiver.begin(IR_LED, false);
	// Initialize other components
	pinMode(LED, OUTPUT);
}

// Loop function: executed in loop
void loop() {
	// Change state in the graph
	switch (currentState) {
	case LISTEN:
		// We wait for a frame
		if (IrReceiver.decode()) {
			// When received, if it is the ON frame
			if (IrReceiver.decodedIRData.command == ON_FRAME) {
				// We switch the state to SWITCH_ON
				currentState = SWITCH_ON;
			}
			// When received, if it is the OFF frame
			else if (IrReceiver.decodedIRData.command == OFF_FRAME) {
				// We switch the state to SWITCH_OFF
				currentState = SWITCH_OFF;
			}
		}
		// Enable receiving of the next value
		IrReceiver.resume();
		break;
	case SWITCH_ON:
		currentState = LISTEN;
		break;
	case SWITCH_OFF:
		currentState = LISTEN;
		break;
	}

	// If state changed run actions
	if (currentState != oldState) {
		// Old state becomes current state
		oldState = currentState;
		switch (currentState) {
		case SWITCH_ON:
			// ON frame received: we set the LED to HIGH
			Serial.print("Received frame form emitter: ");
			Serial.println(IrReceiver.decodedIRData.command, BIN);
			digitalWrite(LED, HIGH);
			break;
		case SWITCH_OFF:
			// OFF frame received: we set the LED to LOW
			Serial.print("Received frame form emitter: ");
			Serial.println(IrReceiver.decodedIRData.command, BIN);
			digitalWrite(LED, LOW);
			break;
		}
	}
}