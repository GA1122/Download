static int dccp_rcv_request_sent_state_process(struct sock *sk,
					       struct sk_buff *skb,
					       const struct dccp_hdr *dh,
					       const unsigned int len)
{
	 
	if (dh->dccph_type == DCCP_PKT_RESPONSE) {
		const struct inet_connection_sock *icsk = inet_csk(sk);
		struct dccp_sock *dp = dccp_sk(sk);
		long tstamp = dccp_timestamp();

		if (!between48(DCCP_SKB_CB(skb)->dccpd_ack_seq,
			       dp->dccps_awl, dp->dccps_awh)) {
			dccp_pr_debug("invalid ackno: S.AWL=%llu, "
				      "P.ackno=%llu, S.AWH=%llu\n",
				      (unsigned long long)dp->dccps_awl,
			   (unsigned long long)DCCP_SKB_CB(skb)->dccpd_ack_seq,
				      (unsigned long long)dp->dccps_awh);
			goto out_invalid_packet;
		}

		 
		if (dccp_parse_options(sk, NULL, skb))
			return 1;

		 
		if (likely(dp->dccps_options_received.dccpor_timestamp_echo))
			dp->dccps_syn_rtt = dccp_sample_rtt(sk, 10 * (tstamp -
			    dp->dccps_options_received.dccpor_timestamp_echo));

		 
		inet_csk_clear_xmit_timer(sk, ICSK_TIME_RETRANS);
		WARN_ON(sk->sk_send_head == NULL);
		kfree_skb(sk->sk_send_head);
		sk->sk_send_head = NULL;

		 
		dp->dccps_gsr = dp->dccps_isr = DCCP_SKB_CB(skb)->dccpd_seq;

		dccp_sync_mss(sk, icsk->icsk_pmtu_cookie);

		 
		dccp_set_state(sk, DCCP_PARTOPEN);

		 
		if (dccp_feat_activate_values(sk, &dp->dccps_featneg))
			goto unable_to_proceed;

		 
		icsk->icsk_af_ops->rebuild_header(sk);

		if (!sock_flag(sk, SOCK_DEAD)) {
			sk->sk_state_change(sk);
			sk_wake_async(sk, SOCK_WAKE_IO, POLL_OUT);
		}

		if (sk->sk_write_pending || icsk->icsk_ack.pingpong ||
		    icsk->icsk_accept_queue.rskq_defer_accept) {
			 
			 
			__kfree_skb(skb);
			return 0;
		}
		dccp_send_ack(sk);
		return -1;
	}

out_invalid_packet:
	 
	DCCP_SKB_CB(skb)->dccpd_reset_code = DCCP_RESET_CODE_PACKET_ERROR;
	return 1;

unable_to_proceed:
	DCCP_SKB_CB(skb)->dccpd_reset_code = DCCP_RESET_CODE_ABORTED;
	 
	dccp_set_state(sk, DCCP_CLOSED);
	sk->sk_err = ECOMM;
	return 1;
}
