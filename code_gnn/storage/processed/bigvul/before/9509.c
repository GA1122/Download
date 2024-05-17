update_local_clock(peer_t *p)
{
	int rc;
	struct timex tmx;
	 
	double offset = p->filter_offset;
	double recv_time = p->lastpkt_recv_time;
	double abs_offset;
#if !USING_KERNEL_PLL_LOOP
	double freq_drift;
#endif
#if !USING_KERNEL_PLL_LOOP || USING_INITIAL_FREQ_ESTIMATION
	double since_last_update;
#endif
	double etemp, dtemp;

	abs_offset = fabs(offset);

#if 0
	 
	 
	if (abs_offset > PANIC_THRESHOLD) {
		bb_error_msg_and_die("offset %f far too big, exiting", offset);
	}
#endif

	 
	if (recv_time <= G.last_update_recv_time) {
		VERB3 bb_error_msg("update from %s: same or older datapoint, not using it",
			p->p_dotted);
		return 0;  
	}

	 
#if !USING_KERNEL_PLL_LOOP || USING_INITIAL_FREQ_ESTIMATION
	since_last_update = recv_time - G.reftime;
#endif
#if !USING_KERNEL_PLL_LOOP
	freq_drift = 0;
#endif
#if USING_INITIAL_FREQ_ESTIMATION
	if (G.discipline_state == STATE_FREQ) {
		 
		if (since_last_update < WATCH_THRESHOLD) {
			VERB4 bb_error_msg("measuring drift, datapoint ignored, %f sec remains",
					WATCH_THRESHOLD - since_last_update);
			return 0;  
		}
# if !USING_KERNEL_PLL_LOOP
		freq_drift = (offset - G.last_update_offset) / since_last_update;
# endif
	}
#endif

	 
	if (abs_offset > STEP_THRESHOLD) {
#if 0
		double remains;

		switch (G.discipline_state) {
		case STATE_SYNC:
			 
			VERB3 bb_error_msg("update from %s: offset:%+f, spike%s",
				p->p_dotted, offset,
				"");
			G.discipline_state = STATE_SPIK;
			return -1;  

		case STATE_SPIK:
			 
			remains = WATCH_THRESHOLD - since_last_update;
			if (remains > 0) {
				VERB3 bb_error_msg("update from %s: offset:%+f, spike%s",
					p->p_dotted, offset,
					", datapoint ignored");
				return -1;  
			}
			 
		}  
#endif

		 
		VERB4 bb_error_msg("stepping time by %+f; poll_exp=MINPOLL", offset);
		step_time(offset);
		if (option_mask32 & OPT_q) {
			 
			exit(0);
		}

		clamp_pollexp_and_set_MAXSTRAT();

		run_script("step", offset);

		recv_time += offset;

#if USING_INITIAL_FREQ_ESTIMATION
		if (G.discipline_state == STATE_NSET) {
			set_new_values(STATE_FREQ,   0, recv_time);
			return 1;  
		}
#endif
		abs_offset = offset = 0;
		set_new_values(STATE_SYNC, offset, recv_time);
	} else {  

		 
		G.offset_to_jitter_ratio = abs_offset / G.discipline_jitter;

		 
		etemp = SQUARE(G.discipline_jitter);
		dtemp = SQUARE(offset - G.last_update_offset);
		G.discipline_jitter = SQRT(etemp + (dtemp - etemp) / AVG);
		if (G.discipline_jitter < G_precision_sec)
			G.discipline_jitter = G_precision_sec;

		switch (G.discipline_state) {
		case STATE_NSET:
			if (option_mask32 & OPT_q) {
				 
				exit(0);
			}
#if USING_INITIAL_FREQ_ESTIMATION
			 
			set_new_values(STATE_FREQ, offset, recv_time);
#else
			set_new_values(STATE_SYNC, offset, recv_time);
#endif
			VERB4 bb_error_msg("transitioning to FREQ, datapoint ignored");
			return 0;  

#if 0  
		case STATE_FSET:
			 
			set_new_values(STATE_SYNC, offset, recv_time);
			 
			break;
#endif

#if USING_INITIAL_FREQ_ESTIMATION
		case STATE_FREQ:
			 
			set_new_values(STATE_SYNC, offset, recv_time);
			break;
#endif

		default:
#if !USING_KERNEL_PLL_LOOP
			 
			if ((1 << G.poll_exp) > ALLAN / 2) {
				etemp = FLL - G.poll_exp;
				if (etemp < AVG)
					etemp = AVG;
				freq_drift += (offset - G.last_update_offset) / (MAXD(since_last_update, ALLAN) * etemp);
			}
			 
			etemp = MIND(since_last_update, (1 << G.poll_exp));
			dtemp = (4 * PLL) << G.poll_exp;
			freq_drift += offset * etemp / SQUARE(dtemp);
#endif
			set_new_values(STATE_SYNC, offset, recv_time);
			break;
		}
		if (G.stratum != p->lastpkt_stratum + 1) {
			G.stratum = p->lastpkt_stratum + 1;
			run_script("stratum", offset);
		}
	}

	G.reftime = G.cur_time;
	G.ntp_status = p->lastpkt_status;
	G.refid = p->lastpkt_refid;
	G.rootdelay = p->lastpkt_rootdelay + p->lastpkt_delay;
	dtemp = p->filter_jitter;  
	dtemp += MAXD(p->filter_dispersion + FREQ_TOLERANCE * (G.cur_time - p->lastpkt_recv_time) + abs_offset, MINDISP);
	G.rootdisp = p->lastpkt_rootdisp + dtemp;
	VERB4 bb_error_msg("updating leap/refid/reftime/rootdisp from peer %s", p->p_dotted);

	 
#if !USING_KERNEL_PLL_LOOP
	 
	dtemp = G.discipline_freq_drift + freq_drift;
	G.discipline_freq_drift = MAXD(MIND(MAXDRIFT, dtemp), -MAXDRIFT);
	etemp = SQUARE(G.discipline_wander);
	dtemp = SQUARE(dtemp);
	G.discipline_wander = SQRT(etemp + (dtemp - etemp) / AVG);

	VERB4 bb_error_msg("discipline freq_drift=%.9f(int:%ld corr:%e) wander=%f",
			G.discipline_freq_drift,
			(long)(G.discipline_freq_drift * 65536e6),
			freq_drift,
			G.discipline_wander);
#endif
	VERB4 {
		memset(&tmx, 0, sizeof(tmx));
		if (adjtimex(&tmx) < 0)
			bb_perror_msg_and_die("adjtimex");
		bb_error_msg("p adjtimex freq:%ld offset:%+ld status:0x%x tc:%ld",
				tmx.freq, tmx.offset, tmx.status, tmx.constant);
	}

	memset(&tmx, 0, sizeof(tmx));
#if 0
	tmx.modes = ADJ_FREQUENCY | ADJ_OFFSET;
	 
	tmx.freq = G.discipline_freq_drift * 65536e6;
#endif
	tmx.modes = ADJ_OFFSET | ADJ_STATUS | ADJ_TIMECONST; 
	tmx.constant = (int)G.poll_exp - 4;
	 
	if (G.offset_to_jitter_ratio >= TIMECONST_HACK_GATE)
		tmx.constant--;
	tmx.offset = (long)(offset * 1000000);  
	if (SLEW_THRESHOLD < STEP_THRESHOLD) {
		if (tmx.offset > (long)(SLEW_THRESHOLD * 1000000)) {
			tmx.offset = (long)(SLEW_THRESHOLD * 1000000);
			tmx.constant--;
		}
		if (tmx.offset < -(long)(SLEW_THRESHOLD * 1000000)) {
			tmx.offset = -(long)(SLEW_THRESHOLD * 1000000);
			tmx.constant--;
		}
	}
	if (tmx.constant < 0)
		tmx.constant = 0;

	tmx.status = STA_PLL;
	if (G.ntp_status & LI_PLUSSEC)
		tmx.status |= STA_INS;
	if (G.ntp_status & LI_MINUSSEC)
		tmx.status |= STA_DEL;

	rc = adjtimex(&tmx);
	if (rc < 0)
		bb_perror_msg_and_die("adjtimex");
	 
	VERB4 bb_error_msg("adjtimex:%d freq:%ld offset:%+ld status:0x%x",
				rc, tmx.freq, tmx.offset, tmx.status);
	G.kernel_freq_drift = tmx.freq / 65536;
	VERB2 bb_error_msg("update from:%s offset:%+f delay:%f jitter:%f clock drift:%+.3fppm tc:%d",
			p->p_dotted,
			offset,
			p->lastpkt_delay,
			G.discipline_jitter,
			(double)tmx.freq / 65536,
			(int)tmx.constant
	);

	return 1;  
}
