int __udp4_lib_rcv(struct sk_buff *skb, struct udp_table *udptable,
		   int proto)
{
	struct sock *sk;
	struct udphdr *uh;
	unsigned short ulen;
	struct rtable *rt = skb_rtable(skb);
	__be32 saddr, daddr;
	struct net *net = dev_net(skb->dev);

	 
	if (!pskb_may_pull(skb, sizeof(struct udphdr)))
		goto drop;		 

	uh   = udp_hdr(skb);
	ulen = ntohs(uh->len);
	saddr = ip_hdr(skb)->saddr;
	daddr = ip_hdr(skb)->daddr;

	if (ulen > skb->len)
		goto short_packet;

	if (proto == IPPROTO_UDP) {
		 
		if (ulen < sizeof(*uh) || pskb_trim_rcsum(skb, ulen))
			goto short_packet;
		uh = udp_hdr(skb);
	}

	if (udp4_csum_init(skb, uh, proto))
		goto csum_error;

	if (skb->sk) {
		int ret;
		sk = skb->sk;

		if (unlikely(sk->sk_rx_dst == NULL))
			udp_sk_rx_dst_set(sk, skb);

		ret = udp_queue_rcv_skb(sk, skb);

		 
		if (ret > 0)
			return -ret;
		return 0;
	} else {
		if (rt->rt_flags & (RTCF_BROADCAST|RTCF_MULTICAST))
			return __udp4_lib_mcast_deliver(net, skb, uh,
					saddr, daddr, udptable);

		sk = __udp4_lib_lookup_skb(skb, uh->source, uh->dest, udptable);
	}

	if (sk != NULL) {
		int ret;

		ret = udp_queue_rcv_skb(sk, skb);
		sock_put(sk);

		 
		if (ret > 0)
			return -ret;
		return 0;
	}

	if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb))
		goto drop;
	nf_reset(skb);

	 
	if (udp_lib_checksum_complete(skb))
		goto csum_error;

	UDP_INC_STATS_BH(net, UDP_MIB_NOPORTS, proto == IPPROTO_UDPLITE);
	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);

	 
	kfree_skb(skb);
	return 0;

short_packet:
	LIMIT_NETDEBUG(KERN_DEBUG "UDP%s: short packet: From %pI4:%u %d/%d to %pI4:%u\n",
		       proto == IPPROTO_UDPLITE ? "Lite" : "",
		       &saddr, ntohs(uh->source),
		       ulen, skb->len,
		       &daddr, ntohs(uh->dest));
	goto drop;

csum_error:
	 
	LIMIT_NETDEBUG(KERN_DEBUG "UDP%s: bad checksum. From %pI4:%u to %pI4:%u ulen %d\n",
		       proto == IPPROTO_UDPLITE ? "Lite" : "",
		       &saddr, ntohs(uh->source), &daddr, ntohs(uh->dest),
		       ulen);
	UDP_INC_STATS_BH(net, UDP_MIB_CSUMERRORS, proto == IPPROTO_UDPLITE);
drop:
	UDP_INC_STATS_BH(net, UDP_MIB_INERRORS, proto == IPPROTO_UDPLITE);
	kfree_skb(skb);
	return 0;
}
