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
				// horrible, yes.
				// TODO: make this better
				for (i = 0; i<60;i++)
					wallclock_inc_sec();
				//wallclock_inc_min();
			break;

			case 'M':
				// horrible, yes.
				// TODO: make this better
				for (i = 0; i<60;i++)
					wallclock_dec_sec();
				//wallclock_dec_min();
			break;

			case 'h':
				//wallclock_inc_hour();
			break;

			case 'H':
				//wallclock_dec_hour();
			break;


			//default:
			//	Serial.write(DEBUG_INVALID_COMMAND);
		}
		update_display();
	}
}


