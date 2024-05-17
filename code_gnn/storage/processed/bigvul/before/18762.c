static void dccp_v6_err(struct sk_buff *skb, struct inet6_skb_parm *opt,
			u8 type, u8 code, int offset, __be32 info)
{
	const struct ipv6hdr *hdr = (const struct ipv6hdr *)skb->data;
	const struct dccp_hdr *dh = (struct dccp_hdr *)(skb->data + offset);
	struct dccp_sock *dp;
	struct ipv6_pinfo *np;
	struct sock *sk;
	int err;
	__u64 seq;
	struct net *net = dev_net(skb->dev);

	if (skb->len < offset + sizeof(*dh) ||
	    skb->len < offset + __dccp_basic_hdr_len(dh)) {
		ICMP6_INC_STATS_BH(net, __in6_dev_get(skb->dev),
				   ICMP6_MIB_INERRORS);
		return;
	}

	sk = inet6_lookup(net, &dccp_hashinfo,
			&hdr->daddr, dh->dccph_dport,
			&hdr->saddr, dh->dccph_sport, inet6_iif(skb));

	if (sk == NULL) {
		ICMP6_INC_STATS_BH(net, __in6_dev_get(skb->dev),
				   ICMP6_MIB_INERRORS);
		return;
	}

	if (sk->sk_state == DCCP_TIME_WAIT) {
		inet_twsk_put(inet_twsk(sk));
		return;
	}

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk))
		NET_INC_STATS_BH(net, LINUX_MIB_LOCKDROPPEDICMPS);

	if (sk->sk_state == DCCP_CLOSED)
		goto out;

	dp = dccp_sk(sk);
	seq = dccp_hdr_seq(dh);
	if ((1 << sk->sk_state) & ~(DCCPF_REQUESTING | DCCPF_LISTEN) &&
	    !between48(seq, dp->dccps_awl, dp->dccps_awh)) {
		NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
		goto out;
	}

	np = inet6_sk(sk);

	if (type == ICMPV6_PKT_TOOBIG) {
		struct dst_entry *dst = NULL;

		if (sock_owned_by_user(sk))
			goto out;
		if ((1 << sk->sk_state) & (DCCPF_LISTEN | DCCPF_CLOSED))
			goto out;

		 
		dst = __sk_dst_check(sk, np->dst_cookie);
		if (dst == NULL) {
			struct inet_sock *inet = inet_sk(sk);
			struct flowi6 fl6;

			 
			memset(&fl6, 0, sizeof(fl6));
			fl6.flowi6_proto = IPPROTO_DCCP;
			ipv6_addr_copy(&fl6.daddr, &np->daddr);
			ipv6_addr_copy(&fl6.saddr, &np->saddr);
			fl6.flowi6_oif = sk->sk_bound_dev_if;
			fl6.fl6_dport = inet->inet_dport;
			fl6.fl6_sport = inet->inet_sport;
			security_sk_classify_flow(sk, flowi6_to_flowi(&fl6));

			dst = ip6_dst_lookup_flow(sk, &fl6, NULL, false);
			if (IS_ERR(dst)) {
				sk->sk_err_soft = -PTR_ERR(dst);
				goto out;
			}
		} else
			dst_hold(dst);

		if (inet_csk(sk)->icsk_pmtu_cookie > dst_mtu(dst)) {
			dccp_sync_mss(sk, dst_mtu(dst));
		}  
		dst_release(dst);
		goto out;
	}

	icmpv6_err_convert(type, code, &err);

	 
	switch (sk->sk_state) {
		struct request_sock *req, **prev;
	case DCCP_LISTEN:
		if (sock_owned_by_user(sk))
			goto out;

		req = inet6_csk_search_req(sk, &prev, dh->dccph_dport,
					   &hdr->daddr, &hdr->saddr,
					   inet6_iif(skb));
		if (req == NULL)
			goto out;

		 
		WARN_ON(req->sk != NULL);

		if (seq != dccp_rsk(req)->dreq_iss) {
			NET_INC_STATS_BH(net, LINUX_MIB_OUTOFWINDOWICMPS);
			goto out;
		}

		inet_csk_reqsk_queue_drop(sk, req, prev);
		goto out;

	case DCCP_REQUESTING:
	case DCCP_RESPOND:   
		if (!sock_owned_by_user(sk)) {
			DCCP_INC_STATS_BH(DCCP_MIB_ATTEMPTFAILS);
			sk->sk_err = err;
			 
			sk->sk_error_report(sk);
			dccp_done(sk);
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