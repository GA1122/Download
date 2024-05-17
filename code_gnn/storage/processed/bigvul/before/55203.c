static int powermate_input_event(struct input_dev *dev, unsigned int type, unsigned int code, int _value)
{
	unsigned int command = (unsigned int)_value;
	struct powermate_device *pm = input_get_drvdata(dev);

	if (type == EV_MSC && code == MSC_PULSELED){
		 
		int static_brightness = command & 0xFF;    
		int pulse_speed = (command >> 8) & 0x1FF;  
		int pulse_table = (command >> 17) & 0x3;   
		int pulse_asleep = (command >> 19) & 0x1;  
		int pulse_awake  = (command >> 20) & 0x1;  

		powermate_pulse_led(pm, static_brightness, pulse_speed, pulse_table, pulse_asleep, pulse_awake);
	}

	return 0;
}
