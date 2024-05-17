void second_overflow(void)
{
	long time_adj;

	 
	time_maxerror += MAXFREQ >> SHIFT_USEC;
	if (time_maxerror > NTP_PHASE_LIMIT) {
		time_maxerror = NTP_PHASE_LIMIT;
		time_status |= STA_UNSYNC;
	}

	 
	switch (time_state) {
	case TIME_OK:
		if (time_status & STA_INS)
			time_state = TIME_INS;
		else if (time_status & STA_DEL)
			time_state = TIME_DEL;
		break;
	case TIME_INS:
		if (xtime.tv_sec % 86400 == 0) {
			xtime.tv_sec--;
			wall_to_monotonic.tv_sec++;
			time_state = TIME_OOP;
			printk(KERN_NOTICE "Clock: inserting leap second "
					"23:59:60 UTC\n");
		}
		break;
	case TIME_DEL:
		if ((xtime.tv_sec + 1) % 86400 == 0) {
			xtime.tv_sec++;
			wall_to_monotonic.tv_sec--;
			time_state = TIME_WAIT;
			printk(KERN_NOTICE "Clock: deleting leap second "
					"23:59:59 UTC\n");
		}
		break;
	case TIME_OOP:
		time_state = TIME_WAIT;
		break;
	case TIME_WAIT:
		if (!(time_status & (STA_INS | STA_DEL)))
		time_state = TIME_OK;
	}

	 
	tick_length = tick_length_base;
	time_adj = shift_right(time_offset, SHIFT_PLL + time_constant);
	time_offset -= time_adj;
	tick_length += (s64)time_adj << (TICK_LENGTH_SHIFT - SHIFT_UPDATE);

	if (unlikely(time_adjust)) {
		if (time_adjust > MAX_TICKADJ) {
			time_adjust -= MAX_TICKADJ;
			tick_length += MAX_TICKADJ_SCALED;
		} else if (time_adjust < -MAX_TICKADJ) {
			time_adjust += MAX_TICKADJ;
			tick_length -= MAX_TICKADJ_SCALED;
		} else {
			tick_length += (s64)(time_adjust * NSEC_PER_USEC /
					NTP_INTERVAL_FREQ) << TICK_LENGTH_SHIFT;
			time_adjust = 0;
		}
	}
}
