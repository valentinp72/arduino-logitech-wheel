const int vibration_button_pin = 2;
const int vibration_out1_pin   = 5;
const int vibration_out2_pin   = 6;
const int vibration_led_pin    = 3; 

int vibration_state = HIGH;
int vibration_previous = LOW;
int vibration_read;

int i_loop = 0;
int i_pwm = 0;

void setup() {
	pinMode(vibration_button_pin, INPUT_PULLUP);
	pinMode(vibration_out1_pin, OUTPUT);
	pinMode(vibration_out2_pin, OUTPUT);
	pinMode(vibration_led_pin,  OUTPUT);
	
	digitalWrite(vibration_led_pin, vibration_state);

	Serial.begin(9600);
}

void loop() {

	vibration_read = digitalRead(vibration_button_pin);
	if (vibration_read == HIGH && vibration_previous == LOW) {
		vibration_state = !vibration_state;
		digitalWrite(vibration_led_pin, !vibration_state);
		Serial.println(vibration_state);
	}
	vibration_previous = vibration_read;

	if (vibration_state == HIGH) {
		analogWrite(vibration_out1_pin, i_pwm);
		analogWrite(vibration_out2_pin, i_pwm);

		if (i_loop > 10) {
			i_pwm = (i_pwm + 1) % 255;
			i_loop = 0;
		} else {
			i_loop += 1;
		}
	} else {
		analogWrite(vibration_out1_pin, 0);
		analogWrite(vibration_out2_pin, 0);
	}

	delay(2);
}
