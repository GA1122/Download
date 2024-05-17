void udp_v4_early_demux(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dev);
	const struct iphdr *iph;
	const struct udphdr *uh;
	struct sock *sk;
	struct dst_entry *dst;
	int dif = skb->dev->ifindex;
	int ours;

	 
	if (!pskb_may_pull(skb, skb_transport_offset(skb) + sizeof(struct udphdr)))
		return;

	iph = ip_hdr(skb);
	uh = udp_hdr(skb);

	if (skb->pkt_type == PACKET_BROADCAST ||
	    skb->pkt_type == PACKET_MULTICAST) {
		struct in_device *in_dev = __in_dev_get_rcu(skb->dev);

		if (!in_dev)
			return;

		ours = ip_check_mc_rcu(in_dev, iph->daddr, iph->saddr,
				       iph->protocol);
		if (!ours)
			return;
		sk = __udp4_lib_mcast_demux_lookup(net, uh->dest, iph->daddr,
						   uh->source, iph->saddr, dif);
	} else if (skb->pkt_type == PACKET_HOST) {
		sk = __udp4_lib_demux_lookup(net, uh->dest, iph->daddr,
					     uh->source, iph->saddr, dif);
	} else {
		return;
	}

	if (!sk)
		return;

	skb->sk = sk;
	skb->destructor = sock_efree;
	dst = READ_ONCE(sk->sk_rx_dst);

	if (dst)
		dst = dst_check(dst, 0);
	if (dst) {
		 
		if (dst->flags & DST_NOCACHE) {
			if (likely(atomic_inc_not_zero(&dst->__refcnt)))
				skb_dst_set(skb, dst);
		} else {
			skb_dst_set_noref(skb, dst);
		}
	}
}
