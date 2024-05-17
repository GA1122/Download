static void _regulator_enable_delay(unsigned int delay)
{
	unsigned int ms = delay / 1000;
	unsigned int us = delay % 1000;

	if (ms > 0) {
		 
		if (ms < 20)
			us += ms * 1000;
		else
			msleep(ms);
	}

	 
	if (us >= 10)
		usleep_range(us, us + 100);
	else
		udelay(us);
}
