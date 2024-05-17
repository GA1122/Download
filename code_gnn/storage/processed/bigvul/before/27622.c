void sequencer_release(int dev, struct file *file)
{
	int i;
	int mode = translate_mode(file);

	dev = dev >> 4;

	DEB(printk("sequencer_release(dev=%d)\n", dev));

	 

	if (mode != OPEN_READ && !(file->f_flags & O_NONBLOCK))
	{
		while (!signal_pending(current) && qlen > 0)
		{
  			seq_sync();
 			interruptible_sleep_on_timeout(&seq_sleeper,
						       3*HZ);
 			 
		}
	}

	if (mode != OPEN_READ)
		seq_drain_midi_queues();	 
	seq_reset();
	if (mode != OPEN_READ)
		seq_drain_midi_queues();	 

	for (i = 0; i < max_synthdev; i++)
	{
		if (synth_open_mask & (1 << i))	 
			if (synth_devs[i])
			{
				synth_devs[i]->close(i);

				module_put(synth_devs[i]->owner);

				if (synth_devs[i]->midi_dev)
					midi_opened[synth_devs[i]->midi_dev] = 0;
			}
	}

	for (i = 0; i < max_mididev; i++)
	{
		if (midi_opened[i]) {
			midi_devs[i]->close(i);
			module_put(midi_devs[i]->owner);
		}
	}

	if (seq_mode == SEQ_2) {
		tmr->close(tmr_no);
		module_put(tmr->owner);
	}

	if (obsolete_api_used)
		printk(KERN_WARNING "/dev/music: Obsolete (4 byte) API was used by %s\n", current->comm);
	sequencer_busy = 0;
}
