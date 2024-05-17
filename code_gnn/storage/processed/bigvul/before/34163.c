static int ccid3_hc_tx_send_packet(struct sock *sk, struct sk_buff *skb)
{
	struct dccp_sock *dp = dccp_sk(sk);
	struct ccid3_hc_tx_sock *hc = ccid3_hc_tx_sk(sk);
	ktime_t now = ktime_get_real();
	s64 delay;

	 
	if (unlikely(skb->len == 0))
		return -EBADMSG;

	if (hc->tx_state == TFRC_SSTATE_NO_SENT) {
		sk_reset_timer(sk, &hc->tx_no_feedback_timer, (jiffies +
			       usecs_to_jiffies(TFRC_INITIAL_TIMEOUT)));
		hc->tx_last_win_count	= 0;
		hc->tx_t_last_win_count = now;

		 
		hc->tx_t_nom = now;

		hc->tx_s = skb->len;

		 
		if (dp->dccps_syn_rtt) {
			ccid3_pr_debug("SYN RTT = %uus\n", dp->dccps_syn_rtt);
			hc->tx_rtt  = dp->dccps_syn_rtt;
			hc->tx_x    = rfc3390_initial_rate(sk);
			hc->tx_t_ld = now;
		} else {
			 
			hc->tx_rtt = DCCP_FALLBACK_RTT;
			hc->tx_x   = hc->tx_s;
			hc->tx_x <<= 6;
		}
		ccid3_update_send_interval(hc);

		ccid3_hc_tx_set_state(sk, TFRC_SSTATE_NO_FBACK);

	} else {
		delay = ktime_us_delta(hc->tx_t_nom, now);
		ccid3_pr_debug("delay=%ld\n", (long)delay);
		 
		if (delay >= TFRC_T_DELTA)
			return (u32)delay / USEC_PER_MSEC;

		ccid3_hc_tx_update_win_count(hc, now);
	}

	 
	dp->dccps_hc_tx_insert_options = 1;
	DCCP_SKB_CB(skb)->dccpd_ccval  = hc->tx_last_win_count;

	 
	hc->tx_t_nom = ktime_add_us(hc->tx_t_nom, hc->tx_t_ipi);
	return CCID_PACKET_SEND_AT_ONCE;
}
