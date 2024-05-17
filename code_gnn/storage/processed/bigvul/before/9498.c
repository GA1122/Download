recv_and_process_peer_pkt(peer_t *p)
{
	int         rc;
	ssize_t     size;
	msg_t       msg;
	double      T1, T2, T3, T4;
	double      offset;
	double      prev_delay, delay;
	unsigned    interval;
	datapoint_t *datapoint;
	peer_t      *q;

	offset = 0;

	 
 recv_again:
	size = recv(p->p_fd, &msg, sizeof(msg), MSG_DONTWAIT);
	if (size < 0) {
		if (errno == EINTR)
			 
			goto recv_again;
		if (errno == EAGAIN)
			 
			return;
		 
		bb_perror_msg_and_die("recv(%s) error", p->p_dotted);
	}

	if (size != NTP_MSGSIZE_NOAUTH && size != NTP_MSGSIZE) {
		bb_error_msg("malformed packet received from %s", p->p_dotted);
		return;
	}

	if (msg.m_orgtime.int_partl != p->p_xmt_msg.m_xmttime.int_partl
	 || msg.m_orgtime.fractionl != p->p_xmt_msg.m_xmttime.fractionl
	) {
		 
		return;
	}

	 
	close(p->p_fd);
	p->p_fd = -1;

	if ((msg.m_status & LI_ALARM) == LI_ALARM
	 || msg.m_stratum == 0
	 || msg.m_stratum > NTP_MAXSTRATUM
	) {
		bb_error_msg("reply from %s: peer is unsynced", p->p_dotted);
		 
		if (G.poll_exp < BIGPOLL)
			goto increase_interval;
		goto pick_normal_interval;
	}


	 
	T1 = p->p_xmttime;
	T2 = lfp_to_d(msg.m_rectime);
	T3 = lfp_to_d(msg.m_xmttime);
	T4 = G.cur_time;

	 
	delay = (T4 - T1) - (T3 - T2);
	if (delay < G_precision_sec)
		delay = G_precision_sec;
	 
	prev_delay = p->p_raw_delay;
	p->p_raw_delay = delay;
	if (p->reachable_bits && delay > prev_delay * BAD_DELAY_GROWTH) {
		bb_error_msg("reply from %s: delay %f is too high, ignoring", p->p_dotted, delay);
		goto pick_normal_interval;
	}

	p->lastpkt_delay = delay;
	p->lastpkt_recv_time = T4;
	VERB6 bb_error_msg("%s->lastpkt_recv_time=%f", p->p_dotted, p->lastpkt_recv_time);
	p->lastpkt_status = msg.m_status;
	p->lastpkt_stratum = msg.m_stratum;
	p->lastpkt_rootdelay = sfp_to_d(msg.m_rootdelay);
	p->lastpkt_rootdisp = sfp_to_d(msg.m_rootdisp);
	p->lastpkt_refid = msg.m_refid;

	p->datapoint_idx = p->reachable_bits ? (p->datapoint_idx + 1) % NUM_DATAPOINTS : 0;
	datapoint = &p->filter_datapoint[p->datapoint_idx];
	datapoint->d_recv_time = T4;
	datapoint->d_offset    = offset = ((T2 - T1) + (T3 - T4)) / 2;
	datapoint->d_dispersion = LOG2D(msg.m_precision_exp) + G_precision_sec;
	if (!p->reachable_bits) {
		 
		int i;
		for (i = 0; i < NUM_DATAPOINTS; i++) {
			p->filter_datapoint[i].d_offset = offset;
		}
	}

	p->reachable_bits |= 1;
	if ((MAX_VERBOSE && G.verbose) || (option_mask32 & OPT_w)) {
		bb_error_msg("reply from %s: offset:%+f delay:%f status:0x%02x strat:%d refid:0x%08x rootdelay:%f reach:0x%02x",
			p->p_dotted,
			offset,
			p->lastpkt_delay,
			p->lastpkt_status,
			p->lastpkt_stratum,
			p->lastpkt_refid,
			p->lastpkt_rootdelay,
			p->reachable_bits
			 
		);
	}

	 
	filter_datapoints(p);
	q = select_and_cluster();
	rc = 0;
	if (q) {
		if (!(option_mask32 & OPT_w)) {
			rc = update_local_clock(q);
#if 0

			 
			if (fabs(q->filter_offset) >= POLLDOWN_OFFSET) {
				VERB4 bb_error_msg("offset:%+f > POLLDOWN_OFFSET", q->filter_offset);
				adjust_poll(-POLLADJ_LIMIT * 3);
				rc = 0;
			}
#endif
		}
	} else {
		 
		if (G.poll_exp < BIGPOLL)
			goto increase_interval;
	}

	if (rc != 0) {
		 
		if (rc > 0 && G.offset_to_jitter_ratio <= POLLADJ_GATE) {
			 
 increase_interval:
			adjust_poll(MINPOLL);
		} else {
			VERB3 if (rc > 0)
				bb_error_msg("want smaller interval: offset/jitter = %u",
					G.offset_to_jitter_ratio);
			adjust_poll(-G.poll_exp * 2);
		}
	}

	 
 pick_normal_interval:
	interval = poll_interval(INT_MAX);
	if (fabs(offset) >= BIGOFF && interval > BIGOFF_INTERVAL) {
		 
		interval = BIGOFF_INTERVAL;
	}

	set_next(p, interval);
}