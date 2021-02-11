#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
	{1, 13, 10, 3},
	{8, 7,  12, 4},
	{2, 15, 11, 14},
	{6, 16, 9,  5}
};

byte rowPins[ROWS] = {4, 7, 8, 9};
byte colPins[COLS] = {10, 14, 15, 16};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
	Serial.begin(9600);
}
  
void loop(){
	int key = (int) keypad.getKey();

	if (key){
		Serial.println(key);
	}

	int state = keypad.getState();
	if (state == PRESSED) {
		Serial.println("key pressed");
	} else if (state == RELEASED ) {
		Serial.println("key released");
	} else if (state == HOLD) {
		Serial.println("key hold");
	}

	delay(50);
}
