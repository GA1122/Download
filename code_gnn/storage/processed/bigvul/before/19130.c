static void tcp_v6_err(struct sk_buff *skb, struct inet6_skb_parm *opt,
		u8 type, u8 code, int offset, __be32 info)
{
	const struct ipv6hdr *hdr = (const struct ipv6hdr*)skb->data;
	const struct tcphdr *th = (struct tcphdr *)(skb->data+offset);
	struct ipv6_pinfo *np;
	struct sock *sk;
	int err;
	struct tcp_sock *tp;
	__u32 seq;
	struct net *net = dev_net(skb->dev);

	sk = inet6_lookup(net, &tcp_hashinfo, &hdr->daddr,
			th->dest, &hdr->saddr, th->source, skb->dev->ifindex);

	if (sk == NULL) {
		ICMP6_INC_STATS_BH(net, __in6_dev_get(skb->dev),
				   ICMP6_MIB_INERRORS);
		return;
	}

	if (sk->sk_state == TCP_TIME_WAIT) {
		inet_twsk_put(inet_twsk(sk));
		return;
	}

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk))
		NET_INC_STATS_BH(net, LINUX_MIB_LOCKDROPPEDICMPS);

	if (sk->sk_state == TCP_CLOSE)
		goto out;

	if (ipv6_hdr(skb)->hop_limit < inet6_sk(sk)->min_hopcount) {
		NET_INC_STATS_BH(net, LINUX_MIB_TCPMINTTLDROP);
		goto out;
	}

	tp = tcp_sk(sk);
	seq = ntohl(th->seq);
	if (sk->sk_state != TCP_LISTEN &&
	    !between(seq, tp->snd_una, tp->snd_nxt)) {
		NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
		goto out;
	}

	np = inet6_sk(sk);

	if (type == ICMPV6_PKT_TOOBIG) {
		struct dst_entry *dst;

		if (sock_owned_by_user(sk))
			goto out;
		if ((1 << sk->sk_state) & (TCPF_LISTEN | TCPF_CLOSE))
			goto out;

		 
		dst = __sk_dst_check(sk, np->dst_cookie);

		if (dst == NULL) {
			struct inet_sock *inet = inet_sk(sk);
			struct flowi6 fl6;

			 
			memset(&fl6, 0, sizeof(fl6));
			fl6.flowi6_proto = IPPROTO_TCP;
			ipv6_addr_copy(&fl6.daddr, &np->daddr);
			ipv6_addr_copy(&fl6.saddr, &np->saddr);
			fl6.flowi6_oif = sk->sk_bound_dev_if;
			fl6.flowi6_mark = sk->sk_mark;
			fl6.fl6_dport = inet->inet_dport;
			fl6.fl6_sport = inet->inet_sport;
			security_skb_classify_flow(skb, flowi6_to_flowi(&fl6));

			dst = ip6_dst_lookup_flow(sk, &fl6, NULL, false);
			if (IS_ERR(dst)) {
				sk->sk_err_soft = -PTR_ERR(dst);
				goto out;
			}

		} else
			dst_hold(dst);

		if (inet_csk(sk)->icsk_pmtu_cookie > dst_mtu(dst)) {
			tcp_sync_mss(sk, dst_mtu(dst));
			tcp_simple_retransmit(sk);
		}  
		dst_release(dst);
		goto out;
	}

	icmpv6_err_convert(type, code, &err);

	 
	switch (sk->sk_state) {
		struct request_sock *req, **prev;
	case TCP_LISTEN:
		if (sock_owned_by_user(sk))
			goto out;

		req = inet6_csk_search_req(sk, &prev, th->dest, &hdr->daddr,
					   &hdr->saddr, inet6_iif(skb));
		if (!req)
			goto out;

		 
		WARN_ON(req->sk != NULL);

		if (seq != tcp_rsk(req)->snt_isn) {
			NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
			goto out;
		}

		inet_csk_reqsk_queue_drop(sk, req, prev);
		goto out;

	case TCP_SYN_SENT:
	case TCP_SYN_RECV:   
		if (!sock_owned_by_user(sk)) {
			sk->sk_err = err;
			sk->sk_error_report(sk);		 

			tcp_done(sk);
		} else
			sk->sk_err_soft = err;
		goto out;
	}

	if (!sock_owned_by_user(sk) && np->recverr) {
		sk->sk_err = err;
		sk->sk_error_report(sk);
	} else
		sk->sk_err_soft = err;

out:
	bh_unlock_sock(sk);
	sock_put(sk);
}