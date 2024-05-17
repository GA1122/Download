void sctp_assoc_rwnd_increase(struct sctp_association *asoc, unsigned int len)
{
	struct sctp_chunk *sack;
	struct timer_list *timer;

	if (asoc->rwnd_over) {
		if (asoc->rwnd_over >= len) {
			asoc->rwnd_over -= len;
		} else {
			asoc->rwnd += (len - asoc->rwnd_over);
			asoc->rwnd_over = 0;
		}
	} else {
		asoc->rwnd += len;
	}

	 
	if (asoc->rwnd_press && asoc->rwnd >= asoc->rwnd_press) {
		int change = min(asoc->pathmtu, asoc->rwnd_press);
		asoc->rwnd += change;
		asoc->rwnd_press -= change;
	}

	pr_debug("%s: asoc:%p rwnd increased by %d to (%u, %u) - %u\n",
		 __func__, asoc, len, asoc->rwnd, asoc->rwnd_over,
		 asoc->a_rwnd);

	 
	if (sctp_peer_needs_update(asoc)) {
		asoc->a_rwnd = asoc->rwnd;

		pr_debug("%s: sending window update SACK- asoc:%p rwnd:%u "
			 "a_rwnd:%u\n", __func__, asoc, asoc->rwnd,
			 asoc->a_rwnd);

		sack = sctp_make_sack(asoc);
		if (!sack)
			return;

		asoc->peer.sack_needed = 0;

		sctp_outq_tail(&asoc->outqueue, sack);

		 
		timer = &asoc->timers[SCTP_EVENT_TIMEOUT_SACK];
		if (del_timer(timer))
			sctp_association_put(asoc);
	}
}
