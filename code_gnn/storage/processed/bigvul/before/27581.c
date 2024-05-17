midi_synth_close(int dev)
{
	int             orig_dev = synth_devs[dev]->midi_dev;

	leave_sysex(dev);

	 
	midi_devs[orig_dev]->outputc(orig_dev, 0xfe);

	midi_devs[orig_dev]->close(orig_dev);
}