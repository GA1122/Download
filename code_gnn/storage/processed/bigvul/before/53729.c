static int tcp_v6_rcv(struct sk_buff *skb)
{
	const struct tcphdr *th;
	const struct ipv6hdr *hdr;
	struct sock *sk;
	int ret;
	struct net *net = dev_net(skb->dev);

	if (skb->pkt_type != PACKET_HOST)
		goto discard_it;

	 
	TCP_INC_STATS_BH(net, TCP_MIB_INSEGS);

	if (!pskb_may_pull(skb, sizeof(struct tcphdr)))
		goto discard_it;

	th = tcp_hdr(skb);

	if (th->doff < sizeof(struct tcphdr)/4)
		goto bad_packet;
	if (!pskb_may_pull(skb, th->doff*4))
		goto discard_it;

	if (skb_checksum_init(skb, IPPROTO_TCP, ip6_compute_pseudo))
		goto csum_error;

	th = tcp_hdr(skb);
	hdr = ipv6_hdr(skb);

lookup:
	sk = __inet6_lookup_skb(&tcp_hashinfo, skb, th->source, th->dest,
				inet6_iif(skb));
	if (!sk)
		goto no_tcp_socket;

process:
	if (sk->sk_state == TCP_TIME_WAIT)
		goto do_time_wait;

	if (sk->sk_state == TCP_NEW_SYN_RECV) {
		struct request_sock *req = inet_reqsk(sk);
		struct sock *nsk = NULL;

		sk = req->rsk_listener;
		tcp_v6_fill_cb(skb, hdr, th);
		if (tcp_v6_inbound_md5_hash(sk, skb)) {
			reqsk_put(req);
			goto discard_it;
		}
		if (likely(sk->sk_state == TCP_LISTEN)) {
			nsk = tcp_check_req(sk, skb, req, false);
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
			tcp_v6_restore_cb(skb);
		} else if (tcp_child_process(sk, nsk, skb)) {
			tcp_v6_send_reset(nsk, skb);
			goto discard_it;
		} else {
			return 0;
		}
	}
	if (hdr->hop_limit < inet6_sk(sk)->min_hopcount) {
		NET_INC_STATS_BH(net, LINUX_MIB_TCPMINTTLDROP);
		goto discard_and_relse;
	}

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb))
		goto discard_and_relse;

	tcp_v6_fill_cb(skb, hdr, th);

	if (tcp_v6_inbound_md5_hash(sk, skb))
		goto discard_and_relse;

	if (sk_filter(sk, skb))
		goto discard_and_relse;

	skb->dev = NULL;

	if (sk->sk_state == TCP_LISTEN) {
		ret = tcp_v6_do_rcv(sk, skb);
		goto put_and_return;
	}

	sk_incoming_cpu_update(sk);

	bh_lock_sock_nested(sk);
	tcp_sk(sk)->segs_in += max_t(u16, 1, skb_shinfo(skb)->gso_segs);
	ret = 0;
	if (!sock_owned_by_user(sk)) {
		if (!tcp_prequeue(sk, skb))
			ret = tcp_v6_do_rcv(sk, skb);
	} else if (unlikely(sk_add_backlog(sk, skb,
					   sk->sk_rcvbuf + sk->sk_sndbuf))) {
		bh_unlock_sock(sk);
		NET_INC_STATS_BH(net, LINUX_MIB_TCPBACKLOGDROP);
		goto discard_and_relse;
	}
	bh_unlock_sock(sk);

put_and_return:
	sock_put(sk);
	return ret ? -1 : 0;

no_tcp_socket:
	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto discard_it;

	tcp_v6_fill_cb(skb, hdr, th);

	if (tcp_checksum_complete(skb)) {
csum_error:
		TCP_INC_STATS_BH(net, TCP_MIB_CSUMERRORS);
bad_packet:
		TCP_INC_STATS_BH(net, TCP_MIB_INERRS);
	} else {
		tcp_v6_send_reset(NULL, skb);
	}

discard_it:
	kfree_skb(skb);
	return 0;

discard_and_relse:
	sock_put(sk);
	goto discard_it;

do_time_wait:
	if (!xfrm6_policy_check(NULL, XFRM_POLICY_IN, skb)) {
		inet_twsk_put(inet_twsk(sk));
		goto discard_it;
	}

	tcp_v6_fill_cb(skb, hdr, th);

	if (tcp_checksum_complete(skb)) {
		inet_twsk_put(inet_twsk(sk));
		goto csum_error;
	}

	switch (tcp_timewait_state_process(inet_twsk(sk), skb, th)) {
	case TCP_TW_SYN:
	{
		struct sock *sk2;

		sk2 = inet6_lookup_listener(dev_net(skb->dev), &tcp_hashinfo,
					    &ipv6_hdr(skb)->saddr, th->source,
					    &ipv6_hdr(skb)->daddr,
					    ntohs(th->dest), tcp_v6_iif(skb));
		if (sk2) {
			struct inet_timewait_sock *tw = inet_twsk(sk);
			inet_twsk_deschedule_put(tw);
			sk = sk2;
			tcp_v6_restore_cb(skb);
			goto process;
		}
		 
	}
	case TCP_TW_ACK:
		tcp_v6_timewait_ack(sk, skb);
		break;
	case TCP_TW_RST:
		tcp_v6_restore_cb(skb);
		goto no_tcp_socket;
	case TCP_TW_SUCCESS:
		;
	}
	goto discard_it;
}