static int seq_timing_event(unsigned char *event_rec)
{
	unsigned char cmd = event_rec[1];
	unsigned int parm = *(int *) &event_rec[4];

	if (seq_mode == SEQ_2)
	{
		int ret;

		if ((ret = tmr->event(tmr_no, event_rec)) == TIMER_ARMED)
			if ((SEQ_MAX_QUEUE - qlen) >= output_threshold)
				wake_up(&seq_sleeper);
		return ret;
	}
	switch (cmd)
	{
		case TMR_WAIT_REL:
			parm += prev_event_time;

			 

		case TMR_WAIT_ABS:
			if (parm > 0)
			{
				long time;

				time = parm;
				prev_event_time = time;

				seq_playing = 1;
				request_sound_timer(time);

				if ((SEQ_MAX_QUEUE - qlen) >= output_threshold)
					wake_up(&seq_sleeper);
				return TIMER_ARMED;
			}
			break;

		case TMR_START:
			seq_time = jiffies;
			prev_input_time = 0;
			prev_event_time = 0;
			break;

		case TMR_STOP:
			break;

		case TMR_CONTINUE:
			break;

		case TMR_TEMPO:
			break;

		case TMR_ECHO:
			if (seq_mode == SEQ_2)
				seq_copy_to_input(event_rec, 8);
			else
			{
				parm = (parm << 8 | SEQ_ECHO);
				seq_copy_to_input((unsigned char *) &parm, 4);
			}
			break;

		default:;
	}

	return TIMER_NOT_ARMED;
}