static void seq_chn_voice_event(unsigned char *event_rec)
{
#define dev event_rec[1]
#define cmd event_rec[2]
#define chn event_rec[3]
#define note event_rec[4]
#define parm event_rec[5]

	int voice = -1;

	if ((int) dev > max_synthdev || synth_devs[dev] == NULL)
		return;
	if (!(synth_open_mask & (1 << dev)))
		return;
	if (!synth_devs[dev])
		return;

	if (seq_mode == SEQ_2)
	{
		if (synth_devs[dev]->alloc_voice)
			voice = find_voice(dev, chn, note);

		if (cmd == MIDI_NOTEON && parm == 0)
		{
			cmd = MIDI_NOTEOFF;
			parm = 64;
		}
	}

	switch (cmd)
	{
		case MIDI_NOTEON:
			if (note > 127 && note != 255)	 
				return;

			if (voice == -1 && seq_mode == SEQ_2 && synth_devs[dev]->alloc_voice)
			{
				 
				voice = alloc_voice(dev, chn, note);
			}
			if (voice == -1)
				voice = chn;

			if (seq_mode == SEQ_2 && (int) dev < num_synths)
			{
				 

				if (chn == 9)
				{
					synth_devs[dev]->set_instr(dev, voice, 128 + note);
					synth_devs[dev]->chn_info[chn].pgm_num = 128 + note;
				}
				synth_devs[dev]->setup_voice(dev, voice, chn);
			}
			synth_devs[dev]->start_note(dev, voice, note, parm);
			break;

		case MIDI_NOTEOFF:
			if (voice == -1)
				voice = chn;
			synth_devs[dev]->kill_note(dev, voice, note, parm);
			break;

		case MIDI_KEY_PRESSURE:
			if (voice == -1)
				voice = chn;
			synth_devs[dev]->aftertouch(dev, voice, parm);
			break;

		default:;
	}
#undef dev
#undef cmd
#undef chn
#undef note
#undef parm
}