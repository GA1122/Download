static void dccp_v6_ctl_send_reset(struct sock *sk, struct sk_buff *rxskb)
{
	const struct ipv6hdr *rxip6h;
	struct sk_buff *skb;
	struct flowi6 fl6;
	struct net *net = dev_net(skb_dst(rxskb)->dev);
	struct sock *ctl_sk = net->dccp.v6_ctl_sk;
	struct dst_entry *dst;

	if (dccp_hdr(rxskb)->dccph_type == DCCP_PKT_RESET)
		return;

	if (!ipv6_unicast_destination(rxskb))
		return;

	skb = dccp_ctl_make_reset(ctl_sk, rxskb);
	if (skb == NULL)
		return;

	rxip6h = ipv6_hdr(rxskb);
	dccp_hdr(skb)->dccph_checksum = dccp_v6_csum_finish(skb, &rxip6h->saddr,
							    &rxip6h->daddr);

	memset(&fl6, 0, sizeof(fl6));
	ipv6_addr_copy(&fl6.daddr, &rxip6h->saddr);
	ipv6_addr_copy(&fl6.saddr, &rxip6h->daddr);

	fl6.flowi6_proto = IPPROTO_DCCP;
	fl6.flowi6_oif = inet6_iif(rxskb);
	fl6.fl6_dport = dccp_hdr(skb)->dccph_dport;
	fl6.fl6_sport = dccp_hdr(skb)->dccph_sport;
	security_skb_classify_flow(rxskb, flowi6_to_flowi(&fl6));

	 
	dst = ip6_dst_lookup_flow(ctl_sk, &fl6, NULL, false);
	if (!IS_ERR(dst)) {
		skb_dst_set(skb, dst);
		ip6_xmit(ctl_sk, skb, &fl6, NULL);
		DCCP_INC_STATS_BH(DCCP_MIB_OUTSEGS);
		DCCP_INC_STATS_BH(DCCP_MIB_OUTRSTS);
		return;
	}

	kfree_skb(skb);
}