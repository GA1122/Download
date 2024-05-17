filter_datapoints(peer_t *p)
{
	int i, idx;
	double sum, wavg;
	datapoint_t *fdp;

#if 0
 
	int got_newest;
	double minoff, maxoff, w;
	double x = x;  
	double oldest_off = oldest_off;
	double oldest_age = oldest_age;
	double newest_off = newest_off;
	double newest_age = newest_age;

	fdp = p->filter_datapoint;

	minoff = maxoff = fdp[0].d_offset;
	for (i = 1; i < NUM_DATAPOINTS; i++) {
		if (minoff > fdp[i].d_offset)
			minoff = fdp[i].d_offset;
		if (maxoff < fdp[i].d_offset)
			maxoff = fdp[i].d_offset;
	}

	idx = p->datapoint_idx;  
	 
	wavg = 0;
	w = 0.5;
	 
	got_newest = 0;
	sum = 0;
	for (i = 0; i < NUM_DATAPOINTS; i++) {
		VERB5 {
			bb_error_msg("datapoint[%d]: off:%f disp:%f(%f) age:%f%s",
				i,
				fdp[idx].d_offset,
				fdp[idx].d_dispersion, dispersion(&fdp[idx]),
				G.cur_time - fdp[idx].d_recv_time,
				(minoff == fdp[idx].d_offset || maxoff == fdp[idx].d_offset)
					? " (outlier by offset)" : ""
			);
		}

		sum += dispersion(&fdp[idx]) / (2 << i);

		if (minoff == fdp[idx].d_offset) {
			minoff -= 1;  
		} else
		if (maxoff == fdp[idx].d_offset) {
			maxoff += 1;
		} else {
			oldest_off = fdp[idx].d_offset;
			oldest_age = G.cur_time - fdp[idx].d_recv_time;
			if (!got_newest) {
				got_newest = 1;
				newest_off = oldest_off;
				newest_age = oldest_age;
			}
			x = oldest_off * w;
			wavg += x;
			w /= 2;
		}

		idx = (idx - 1) & (NUM_DATAPOINTS - 1);
	}
	p->filter_dispersion = sum;
	wavg += x;  
	 
	x = oldest_age - newest_age;
	if (x != 0) {
		x = newest_age / x;  
		if (x < 1) {  
			x = (newest_off - oldest_off) * x;  
			wavg += x;
		}
	}
	p->filter_offset = wavg;

#else

	fdp = p->filter_datapoint;
	idx = p->datapoint_idx;  

	 
	p->filter_offset = fdp[idx].d_offset;

	 
	wavg = 0;
	sum = 0;
	for (i = 0; i < NUM_DATAPOINTS; i++) {
		sum += dispersion(&fdp[idx]) / (2 << i);
		wavg += fdp[idx].d_offset;
		idx = (idx - 1) & (NUM_DATAPOINTS - 1);
	}
	wavg /= NUM_DATAPOINTS;
	p->filter_dispersion = sum;
#endif

	 
	sum = 0;
	for (i = 0; i < NUM_DATAPOINTS; i++) {
		sum += SQUARE(wavg - fdp[i].d_offset);
	}
	sum = SQRT(sum / NUM_DATAPOINTS);
	p->filter_jitter = sum > G_precision_sec ? sum : G_precision_sec;

	VERB4 bb_error_msg("filter offset:%+f disp:%f jitter:%f",
			p->filter_offset,
			p->filter_dispersion,
			p->filter_jitter);
}
