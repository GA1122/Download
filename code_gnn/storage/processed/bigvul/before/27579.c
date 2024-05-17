midi_outc(int midi_dev, int data)
{
	int             timeout;

	for (timeout = 0; timeout < 3200; timeout++)
		if (midi_devs[midi_dev]->outputc(midi_dev, (unsigned char) (data & 0xff)))
		  {
			  if (data & 0x80)	 
				  prev_out_status[midi_dev] =
				      (unsigned char) (data & 0xff);	 
			  return;	 
		  }
	 
	printk("Midi send timed out\n");
}