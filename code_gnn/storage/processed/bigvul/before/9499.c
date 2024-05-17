reset_peer_stats(peer_t *p, double offset)
{
	int i;
	bool small_ofs = fabs(offset) < STEP_THRESHOLD;

	 

	for (i = 0; i < NUM_DATAPOINTS; i++) {
		if (small_ofs) {
			p->filter_datapoint[i].d_recv_time += offset;
			if (p->filter_datapoint[i].d_offset != 0) {
				p->filter_datapoint[i].d_offset -= offset;
			}
		} else {
			p->filter_datapoint[i].d_recv_time  = G.cur_time;
			p->filter_datapoint[i].d_offset     = 0;
			 
		}
	}
	if (small_ofs) {
		p->lastpkt_recv_time += offset;
	} else {
		 
		p->lastpkt_recv_time = G.cur_time;
	}
	filter_datapoints(p);  
	VERB6 bb_error_msg("%s->lastpkt_recv_time=%f", p->p_dotted, p->lastpkt_recv_time);
}
