adjust_poll(int count)
{
	G.polladj_count += count;
	if (G.polladj_count > POLLADJ_LIMIT) {
		G.polladj_count = 0;
		if (G.poll_exp < MAXPOLL) {
			G.poll_exp++;
			VERB4 bb_error_msg("polladj: discipline_jitter:%f ++poll_exp=%d",
					G.discipline_jitter, G.poll_exp);
		}
	} else if (G.polladj_count < -POLLADJ_LIMIT || (count < 0 && G.poll_exp > BIGPOLL)) {
		G.polladj_count = 0;
		if (G.poll_exp > MINPOLL) {
			llist_t *item;

			G.poll_exp--;
			 
			for (item = G.ntp_peers; item != NULL; item = item->link) {
				peer_t *pp = (peer_t *) item->data;
				if (pp->p_fd < 0)
					pp->next_action_time -= (1 << G.poll_exp);
			}
			VERB4 bb_error_msg("polladj: discipline_jitter:%f --poll_exp=%d",
					G.discipline_jitter, G.poll_exp);
		}
	} else {
		VERB4 bb_error_msg("polladj: count:%d", G.polladj_count);
	}
}
