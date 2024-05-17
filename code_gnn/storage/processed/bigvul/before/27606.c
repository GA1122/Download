static void seq_chn_common_event(unsigned char *event_rec)
{
	unsigned char dev = event_rec[1];
	unsigned char cmd = event_rec[2];
	unsigned char chn = event_rec[3];
	unsigned char p1 = event_rec[4];

	 
	unsigned short w14 = *(short *) &event_rec[6];

	if ((int) dev > max_synthdev || synth_devs[dev] == NULL)
		return;
	if (!(synth_open_mask & (1 << dev)))
		return;
	if (!synth_devs[dev])
		return;

	switch (cmd)
	{
		case MIDI_PGM_CHANGE:
			if (seq_mode == SEQ_2)
			{
				synth_devs[dev]->chn_info[chn].pgm_num = p1;
				if ((int) dev >= num_synths)
					synth_devs[dev]->set_instr(dev, chn, p1);
			}
			else
				synth_devs[dev]->set_instr(dev, chn, p1);

			break;

		case MIDI_CTL_CHANGE:
			if (seq_mode == SEQ_2)
			{
				if (chn > 15 || p1 > 127)
					break;

				synth_devs[dev]->chn_info[chn].controllers[p1] = w14 & 0x7f;

				if (p1 < 32)	 
					synth_devs[dev]->chn_info[chn].controllers[p1 + 32] = 0;

				if ((int) dev < num_synths)
				{
					int val = w14 & 0x7f;
					int i, key;

					if (p1 < 64)	 
					{
						val = ((synth_devs[dev]->
							chn_info[chn].controllers[p1 & ~32] & 0x7f) << 7)
							| (synth_devs[dev]->
							chn_info[chn].controllers[p1 | 32] & 0x7f);
						p1 &= ~32;
					}
					 

					key = ((int) chn << 8);

					for (i = 0; i < synth_devs[dev]->alloc.max_voice; i++)
						if ((synth_devs[dev]->alloc.map[i] & 0xff00) == key)
							synth_devs[dev]->controller(dev, i, p1, val);
				}
				else
					synth_devs[dev]->controller(dev, chn, p1, w14);
			}
			else	 
				synth_devs[dev]->controller(dev, chn, p1, w14);
			break;

		case MIDI_PITCH_BEND:
			if (seq_mode == SEQ_2)
			{
				synth_devs[dev]->chn_info[chn].bender_value = w14;

				if ((int) dev < num_synths)
				{
					 
					int i, key;

					key = (chn << 8);

					for (i = 0; i < synth_devs[dev]->alloc.max_voice; i++)
						if ((synth_devs[dev]->alloc.map[i] & 0xff00) == key)
							synth_devs[dev]->bender(dev, i, w14);
				}
				else
					synth_devs[dev]->bender(dev, chn, w14);
			}
			else	 
				synth_devs[dev]->bender(dev, chn, w14);
			break;

		default:;
	}
}