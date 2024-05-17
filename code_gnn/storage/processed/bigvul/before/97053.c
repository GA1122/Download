int ip6_xmit(struct sock *sk, struct sk_buff *skb, struct flowi6 *fl6,
	     struct ipv6_txoptions *opt, int tclass)
{
	struct net *net = sock_net(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct in6_addr *first_hop = &fl6->daddr;
	struct dst_entry *dst = skb_dst(skb);
	unsigned int head_room;
	struct ipv6hdr *hdr;
	u8  proto = fl6->flowi6_proto;
	int seg_len = skb->len;
	int hlimit = -1;
	u32 mtu;

	head_room = sizeof(struct ipv6hdr) + LL_RESERVED_SPACE(dst->dev);
	if (opt)
		head_room += opt->opt_nflen + opt->opt_flen;

	if (unlikely(skb_headroom(skb) < head_room)) {
		struct sk_buff *skb2 = skb_realloc_headroom(skb, head_room);
		if (!skb2) {
			IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)),
				      IPSTATS_MIB_OUTDISCARDS);
			kfree_skb(skb);
			return -ENOBUFS;
		}
		if (skb->sk)
			skb_set_owner_w(skb2, skb->sk);
		consume_skb(skb);
		skb = skb2;
	}

	if (opt) {
		seg_len += opt->opt_nflen + opt->opt_flen;

		if (opt->opt_flen)
			ipv6_push_frag_opts(skb, opt, &proto);

		if (opt->opt_nflen)
			ipv6_push_nfrag_opts(skb, opt, &proto, &first_hop);
	}

	skb_push(skb, sizeof(struct ipv6hdr));
	skb_reset_network_header(skb);
	hdr = ipv6_hdr(skb);

	 
	if (np)
		hlimit = np->hop_limit;
	if (hlimit < 0)
		hlimit = ip6_dst_hoplimit(dst);

	ip6_flow_hdr(hdr, tclass, ip6_make_flowlabel(net, skb, fl6->flowlabel,
						     np->autoflowlabel));

	hdr->payload_len = htons(seg_len);
	hdr->nexthdr = proto;
	hdr->hop_limit = hlimit;

	hdr->saddr = fl6->saddr;
	hdr->daddr = *first_hop;

	skb->protocol = htons(ETH_P_IPV6);
	skb->priority = sk->sk_priority;
	skb->mark = sk->sk_mark;

	mtu = dst_mtu(dst);
	if ((skb->len <= mtu) || skb->ignore_df || skb_is_gso(skb)) {
		IP6_UPD_PO_STATS(net, ip6_dst_idev(skb_dst(skb)),
			      IPSTATS_MIB_OUT, skb->len);
		return NF_HOOK(NFPROTO_IPV6, NF_INET_LOCAL_OUT, skb, NULL,
			       dst->dev, dst_output);
	}

	skb->dev = dst->dev;
	ipv6_local_error(sk, EMSGSIZE, fl6, mtu);
	IP6_INC_STATS(net, ip6_dst_idev(skb_dst(skb)), IPSTATS_MIB_FRAGFAILS);
	kfree_skb(skb);
	return -EMSGSIZE;
}
