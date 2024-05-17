static void midi_outc(int dev, unsigned char data)
{
	 

	int n;
	unsigned long flags;

	 

	n = 3 * HZ;		 

	spin_lock_irqsave(&lock,flags);
 	while (n && !midi_devs[dev]->outputc(dev, data)) {
 		interruptible_sleep_on_timeout(&seq_sleeper, HZ/25);
  		n--;
  	}
	spin_unlock_irqrestore(&lock,flags);
}
