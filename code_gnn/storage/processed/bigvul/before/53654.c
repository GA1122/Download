static int dccp_v6_rcv(struct sk_buff *skb)
{
	const struct dccp_hdr *dh;
	struct sock *sk;
	int min_cov;

	 

	if (dccp_invalid_packet(skb))
		goto discard_it;

	 
	if (dccp_v6_csum_finish(skb, &ipv6_hdr(skb)->saddr,
				     &ipv6_hdr(skb)->daddr)) {
		DCCP_WARN("dropped packet with invalid checksum\n");
		goto discard_it;
	}

	dh = dccp_hdr(skb);

	DCCP_SKB_CB(skb)->dccpd_seq  = dccp_hdr_seq(dh);
	DCCP_SKB_CB(skb)->dccpd_type = dh->dccph_type;

	if (dccp_packet_without_ack(skb))
		DCCP_SKB_CB(skb)->dccpd_ack_seq = DCCP_PKT_WITHOUT_ACK_SEQ;
	else
		DCCP_SKB_CB(skb)->dccpd_ack_seq = dccp_hdr_ack_seq(skb);

lookup:
	sk = __inet6_lookup_skb(&dccp_hashinfo, skb,
			        dh->dccph_sport, dh->dccph_dport,
				inet6_iif(skb));
	if (!sk) {
		dccp_pr_debug("failed to look up flow ID in table and "
			      "get corresponding socket\n");
		goto no_dccp_socket;
	}

	 
	if (sk->sk_state == DCCP_TIME_WAIT) {
		dccp_pr_debug("sk->sk_state == DCCP_TIME_WAIT: do_time_wait\n");
		inet_twsk_put(inet_twsk(sk));
		goto no_dccp_socket;
	}

	if (sk->sk_state == DCCP_NEW_SYN_RECV) {
		struct request_sock *req = inet_reqsk(sk);
		struct sock *nsk = NULL;

		sk = req->rsk_listener;
		if (likely(sk->sk_state == DCCP_LISTEN)) {
			nsk = dccp_check_req(sk, skb, req);
		} else {
			inet_csk_reqsk_queue_drop_and_put(sk, req);
			goto lookup;
		}
		if (!nsk) {
			reqsk_put(req);
			goto discard_it;
		}
		if (nsk == sk) {
			sock_hold(sk);
			reqsk_put(req);
		} else if (dccp_child_process(sk, nsk, skb)) {
			dccp_v6_ctl_send_reset(sk, skb);
			goto discard_it;
		} else {
			return 0;
		}
	}
	 
	min_cov = dccp_sk(sk)->dccps_pcrlen;
	if (dh->dccph_cscov  &&  (min_cov == 0 || dh->dccph_cscov < min_cov))  {
		dccp_pr_debug("Packet CsCov %d does not satisfy MinCsCov %d\n",
			      dh->dccph_cscov, min_cov);
		 
		goto discard_and_relse;
	}

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb))
		goto discard_and_relse;

	return sk_receive_skb(sk, skb, 1) ? -1 : 0;

no_dccp_socket:
	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard_it;
	 
	if (dh->dccph_type != DCCP_PKT_RESET) {
		DCCP_SKB_CB(skb)->dccpd_reset_code =
					DCCP_RESET_CODE_NO_CONNECTION;
		dccp_v6_ctl_send_reset(sk, skb);
	}

discard_it:
	kfree_skb(skb);
	return 0;

discard_and_relse:
	sock_put(sk);
	goto discard_it;
}