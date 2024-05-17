fit(peer_t *p, double rd)
{
	if ((p->reachable_bits & (p->reachable_bits-1)) == 0) {
		 
		VERB4 bb_error_msg("peer %s unfit for selection: unreachable", p->p_dotted);
		return 0;
	}
#if 0  
	if ((p->lastpkt_status & LI_ALARM) == LI_ALARM
	 || p->lastpkt_stratum >= MAXSTRAT
	) {
		VERB4 bb_error_msg("peer %s unfit for selection: bad status/stratum", p->p_dotted);
		return 0;
	}
#endif
	 
	if (rd > MAXDIST + FREQ_TOLERANCE * (1 << G.poll_exp)) {
		VERB4 bb_error_msg("peer %s unfit for selection: root distance too high", p->p_dotted);
		return 0;
	}
	return 1;
}