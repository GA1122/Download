root_distance(peer_t *p)
{
	 
	return MAXD(MINDISP, p->lastpkt_rootdelay + p->lastpkt_delay) / 2
		+ p->lastpkt_rootdisp
		+ p->filter_dispersion
		+ FREQ_TOLERANCE * (G.cur_time - p->lastpkt_recv_time)
		+ p->filter_jitter;
}
