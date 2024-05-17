static int play_event(unsigned char *q)
{
	 
	unsigned int *delay;

	switch (q[0])
	{
		case SEQ_NOTEOFF:
			if (synth_open_mask & (1 << 0))
				if (synth_devs[0])
					synth_devs[0]->kill_note(0, q[1], 255, q[3]);
			break;

		case SEQ_NOTEON:
			if (q[4] < 128 || q[4] == 255)
				if (synth_open_mask & (1 << 0))
					if (synth_devs[0])
						synth_devs[0]->start_note(0, q[1], q[2], q[3]);
			break;

		case SEQ_WAIT:
			delay = (unsigned int *) q;	 
			*delay = (*delay >> 8) & 0xffffff;

			if (*delay > 0)
			{
				long time;

				seq_playing = 1;
				time = *delay;
				prev_event_time = time;

				request_sound_timer(time);

				if ((SEQ_MAX_QUEUE - qlen) >= output_threshold)
					wake_up(&seq_sleeper);
				 
				return 1;
			}
			break;

		case SEQ_PGMCHANGE:
			if (synth_open_mask & (1 << 0))
				if (synth_devs[0])
					synth_devs[0]->set_instr(0, q[1], q[2]);
			break;

		case SEQ_SYNCTIMER: 	 
			seq_time = jiffies;
			prev_input_time = 0;
			prev_event_time = 0;
			break;

		case SEQ_MIDIPUTC:	 
			if (midi_opened[q[2]])
			{
				int dev;

				dev = q[2];

				if (dev < 0 || dev >= num_midis || midi_devs[dev] == NULL)
					break;

				if (!midi_devs[dev]->outputc(dev, q[1]))
				{
					 

					seq_playing = 1;
					request_sound_timer(-1);
					return 2;
				}
				else
					midi_written[dev] = 1;
			}
			break;

		case SEQ_ECHO:
			seq_copy_to_input(q, 4);	 
			break;

		case SEQ_PRIVATE:
			if ((int) q[1] < max_synthdev)
				synth_devs[q[1]]->hw_control(q[1], q);
			break;

		case SEQ_EXTENDED:
			extended_event(q);
			break;

		case EV_CHN_VOICE:
			seq_chn_voice_event(q);
			break;

		case EV_CHN_COMMON:
			seq_chn_common_event(q);
			break;

		case EV_TIMING:
			if (seq_timing_event(q) == TIMER_ARMED)
			{
				return 1;
			}
			break;

		case EV_SEQ_LOCAL:
			seq_local_event(q);
			break;

		case EV_SYSEX:
			seq_sysex_message(q);
			break;

		default:;
	}
	return 0;
}