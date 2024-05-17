static void ipv4_link_failure(struct sk_buff *skb)
{
	struct rtable *rt;

	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_HOST_UNREACH, 0);

	rt = skb_rtable(skb);
	if (rt && rt->peer && peer_pmtu_cleaned(rt->peer))
		dst_metric_set(&rt->dst, RTAX_MTU, rt->peer->pmtu_orig);
}
