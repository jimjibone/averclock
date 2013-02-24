void remote_init(void);
void remote_command(void);



void remote_init(void) {
	Serial.begin(9600);
}

void remote_command(void) {
	uint8_t command = 0x00;
	uint8_t i;
	if (Serial.available()) {
		command = Serial.read();
		switch (command) {
			case 'm':
				inc_time();
			break;

			case 'M':
				dec_time();
			break;

			case 'h':
				// horrible, yes.
				for (i = 0; i<60;i++)
					inc_time();
			break;

			case 'H':
				// horrible, yes.
				for (i = 0; i<60;i++)
					dec_time();
			break;


			//default:
			//	Serial.write(DEBUG_INVALID_COMMAND);
		}
		update_display();
	}
}


