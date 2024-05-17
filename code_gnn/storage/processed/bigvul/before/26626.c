int ip6_push_pending_frames(struct sock *sk)
{
	struct sk_buff *skb, *tmp_skb;
	struct sk_buff **tail_skb;
	struct in6_addr final_dst_buf, *final_dst = &final_dst_buf;
	struct inet_sock *inet = inet_sk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct net *net = sock_net(sk);
	struct ipv6hdr *hdr;
	struct ipv6_txoptions *opt = np->cork.opt;
	struct rt6_info *rt = (struct rt6_info *)inet->cork.base.dst;
	struct flowi6 *fl6 = &inet->cork.fl.u.ip6;
	unsigned char proto = fl6->flowi6_proto;
	int err = 0;

	if ((skb = __skb_dequeue(&sk->sk_write_queue)) == NULL)
		goto out;
	tail_skb = &(skb_shinfo(skb)->frag_list);

	 
	if (skb->data < skb_network_header(skb))
		__skb_pull(skb, skb_network_offset(skb));
	while ((tmp_skb = __skb_dequeue(&sk->sk_write_queue)) != NULL) {
		__skb_pull(tmp_skb, skb_network_header_len(skb));
		*tail_skb = tmp_skb;
		tail_skb = &(tmp_skb->next);
		skb->len += tmp_skb->len;
		skb->data_len += tmp_skb->len;
		skb->truesize += tmp_skb->truesize;
		tmp_skb->destructor = NULL;
		tmp_skb->sk = NULL;
	}

	 
	if (np->pmtudisc < IPV6_PMTUDISC_DO)
		skb->local_df = 1;

	ipv6_addr_copy(final_dst, &fl6->daddr);
	__skb_pull(skb, skb_network_header_len(skb));
	if (opt && opt->opt_flen)
		ipv6_push_frag_opts(skb, opt, &proto);
	if (opt && opt->opt_nflen)
		ipv6_push_nfrag_opts(skb, opt, &proto, &final_dst);

	skb_push(skb, sizeof(struct ipv6hdr));
	skb_reset_network_header(skb);
	hdr = ipv6_hdr(skb);

	*(__be32*)hdr = fl6->flowlabel |
		     htonl(0x60000000 | ((int)np->cork.tclass << 20));

	hdr->hop_limit = np->cork.hop_limit;
	hdr->nexthdr = proto;
	ipv6_addr_copy(&hdr->saddr, &fl6->saddr);
	ipv6_addr_copy(&hdr->daddr, final_dst);

	skb->priority = sk->sk_priority;
	skb->mark = sk->sk_mark;

	skb_dst_set(skb, dst_clone(&rt->dst));
	IP6_UPD_PO_STATS(net, rt->rt6i_idev, IPSTATS_MIB_OUT, skb->len);
	if (proto == IPPROTO_ICMPV6) {
		struct inet6_dev *idev = ip6_dst_idev(skb_dst(skb));

		ICMP6MSGOUT_INC_STATS_BH(net, idev, icmp6_hdr(skb)->icmp6_type);
		ICMP6_INC_STATS_BH(net, idev, ICMP6_MIB_OUTMSGS);
	}

	err = ip6_local_out(skb);
	if (err) {
		if (err > 0)
			err = net_xmit_errno(err);
		if (err)
			goto error;
	}

out:
	ip6_cork_release(inet, np);
	return err;
error:
	IP6_INC_STATS(net, rt->rt6i_idev, IPSTATS_MIB_OUTDISCARDS);
	goto out;
}