midi_synth_open(int dev, int mode)
{
	int             orig_dev = synth_devs[dev]->midi_dev;
	int             err;
	struct midi_input_info *inc;

	if (orig_dev < 0 || orig_dev >= num_midis || midi_devs[orig_dev] == NULL)
		return -ENXIO;

	midi2synth[orig_dev] = dev;
	sysex_state[dev] = 0;
	prev_out_status[orig_dev] = 0;

	if ((err = midi_devs[orig_dev]->open(orig_dev, mode,
			       midi_synth_input, midi_synth_output)) < 0)
		return err;
	inc = &midi_devs[orig_dev]->in_info;

	 
	inc->m_busy = 0;
	inc->m_state = MST_INIT;
	inc->m_ptr = 0;
	inc->m_left = 0;
	inc->m_prev_status = 0x00;
	 

	return 1;
}