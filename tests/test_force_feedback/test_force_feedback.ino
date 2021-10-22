#include <Joystick.h>
#include <Keypad.h>

const int accel_pin = A0;
const int brake_pin = A1;
const int wheel_pin = A2;

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
const int vibration_led_pin = 3; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Joystick_ Joystick(
	JOYSTICK_DEFAULT_REPORT_ID,
	JOYSTICK_TYPE_JOYSTICK,
	16,                   // button count
	0,                    // hat switch count
	true, true, true,     // X Y Z axis
	false, false, false,     // rX rY rZ axis
	false,                 // rudder
	false,                 // throttle
	false,                 // accelerator
	false,                 // brake
	false                  // steering
);

float EMA_a = 0.25;
int EMA_s = 0;

void setup() {
	Serial.begin(9600);
	Joystick.begin();
	Joystick.setXAxisRange(-512, 511);
	Joystick.setYAxisRange(0, 1023);
	pinMode(vibration_led_pin, OUTPUT);
	pinMode(2, INPUT_PULLUP);

	EMA_s = map(analogRead(wheel_pin), 0, 1023, -512, 511);
}

int old_wheel = 0;
int old_accel = 0;
int old_brake = 0;

int last_key = 0;

void loop() {
	int key = (int) keypad.getKey();

	if (key) {
		if (keypad.getState() == PRESSED) {
			last_key = key - 1;
			Joystick.setButton(key - 1, HIGH);
		}
	}
	if (keypad.getState() == RELEASED) {
		Joystick.setButton(last_key, LOW);
	}

	int wheel = map(analogRead(wheel_pin), 0, 1023, -512, 511);
	EMA_s = (EMA_a * wheel) + ((1 - EMA_a) * EMA_s);
	wheel = EMA_s;
	if (wheel != old_wheel) {
		Joystick.setXAxis(wheel);
		old_wheel = wheel;
		Serial.print("-512,512,");
		Serial.println(wheel);
	}

	int accel = analogRead(accel_pin);
	if (accel != old_accel) {
		Joystick.setYAxis(accel);
		old_accel = accel;
	}

	int brake = analogRead(brake_pin);
	if (brake != old_brake) {
		Joystick.setZAxis(brake);
		old_brake = brake;
	}

	delay(50);
}

