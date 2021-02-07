#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
	{'1','U','a','3'},
	{'8','7','c','4'},
	{'2','L','b','D'},
	{'+','R','9','-'}
};

byte rowPins[ROWS] = {4, 7, 8, 9};
byte colPins[COLS] = {10, 14, 15, 16};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
	Serial.begin(9600);
}
  
void loop(){
	char key = keypad.getKey();

	if (key){
		Serial.println(key);
	}
}
