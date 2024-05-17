static void seq_drain_midi_queues(void)
{
	int i, n;

	 

	n = 1;

	while (!signal_pending(current) && n)
	{
		n = 0;

		for (i = 0; i < max_mididev; i++)
			if (midi_opened[i] && midi_written[i])
				if (midi_devs[i]->buffer_status != NULL)
					if (midi_devs[i]->buffer_status(i))
						n++;

		 

 		if (n)
 			interruptible_sleep_on_timeout(&seq_sleeper,
						       HZ/10);
	}
}
