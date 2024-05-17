static void ccid3_hc_tx_packet_recv(struct sock *sk, struct sk_buff *skb)
{
	struct ccid3_hc_tx_sock *hc = ccid3_hc_tx_sk(sk);
	struct tfrc_tx_hist_entry *acked;
	ktime_t now;
	unsigned long t_nfb;
	u32 r_sample;

	 
	if (!(DCCP_SKB_CB(skb)->dccpd_type == DCCP_PKT_ACK ||
	      DCCP_SKB_CB(skb)->dccpd_type == DCCP_PKT_DATAACK))
		return;
	 
	acked = tfrc_tx_hist_find_entry(hc->tx_hist, dccp_hdr_ack_seq(skb));
	if (acked == NULL)
		return;
	 
	tfrc_tx_hist_purge(&acked->next);

	 
	now	  = ktime_get_real();
	r_sample  = dccp_sample_rtt(sk, ktime_us_delta(now, acked->stamp));
	hc->tx_rtt = tfrc_ewma(hc->tx_rtt, r_sample, 9);

	 
	if (hc->tx_state == TFRC_SSTATE_NO_FBACK) {
		ccid3_hc_tx_set_state(sk, TFRC_SSTATE_FBACK);

		if (hc->tx_t_rto == 0) {
			 
			hc->tx_x    = rfc3390_initial_rate(sk);
			hc->tx_t_ld = now;

			ccid3_update_send_interval(hc);

			goto done_computing_x;
		} else if (hc->tx_p == 0) {
			 
			goto done_computing_x;
		}
	}

	 
	if (hc->tx_p > 0)
		hc->tx_x_calc = tfrc_calc_x(hc->tx_s, hc->tx_rtt, hc->tx_p);
	ccid3_hc_tx_update_x(sk, &now);

done_computing_x:
	ccid3_pr_debug("%s(%p), RTT=%uus (sample=%uus), s=%u, "
			       "p=%u, X_calc=%u, X_recv=%u, X=%u\n",
			       dccp_role(sk), sk, hc->tx_rtt, r_sample,
			       hc->tx_s, hc->tx_p, hc->tx_x_calc,
			       (unsigned int)(hc->tx_x_recv >> 6),
			       (unsigned int)(hc->tx_x >> 6));

	 
	sk_stop_timer(sk, &hc->tx_no_feedback_timer);

	 
	sk->sk_write_space(sk);

	 
	hc->tx_t_rto = max_t(u32, 4 * hc->tx_rtt,
				  USEC_PER_SEC/HZ * tcp_rto_min(sk));
	 
	t_nfb = max(hc->tx_t_rto, 2 * hc->tx_t_ipi);

	ccid3_pr_debug("%s(%p), Scheduled no feedback timer to "
		       "expire in %lu jiffies (%luus)\n",
		       dccp_role(sk), sk, usecs_to_jiffies(t_nfb), t_nfb);

	sk_reset_timer(sk, &hc->tx_no_feedback_timer,
			   jiffies + usecs_to_jiffies(t_nfb));
}
