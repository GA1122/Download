int ip_options_rcv_srr(struct sk_buff *skb)
{
	struct ip_options *opt = &(IPCB(skb)->opt);
	int srrspace, srrptr;
	__be32 nexthop;
	struct iphdr *iph = ip_hdr(skb);
	unsigned char *optptr = skb_network_header(skb) + opt->srr;
	struct rtable *rt = skb_rtable(skb);
	struct rtable *rt2;
	unsigned long orefdst;
	int err;

	if (!opt->srr || !rt)
		return 0;

	if (skb->pkt_type != PACKET_HOST)
		return -EINVAL;
	if (rt->rt_type == RTN_UNICAST) {
		if (!opt->is_strictroute)
			return 0;
		icmp_send(skb, ICMP_PARAMETERPROB, 0, htonl(16<<24));
		return -EINVAL;
	}
	if (rt->rt_type != RTN_LOCAL)
		return -EINVAL;

	for (srrptr=optptr[2], srrspace = optptr[1]; srrptr <= srrspace; srrptr += 4) {
		if (srrptr + 3 > srrspace) {
			icmp_send(skb, ICMP_PARAMETERPROB, 0, htonl((opt->srr+2)<<24));
			return -EINVAL;
		}
		memcpy(&nexthop, &optptr[srrptr-1], 4);

		orefdst = skb->_skb_refdst;
		skb_dst_set(skb, NULL);
		err = ip_route_input(skb, nexthop, iph->saddr, iph->tos, skb->dev);
		rt2 = skb_rtable(skb);
		if (err || (rt2->rt_type != RTN_UNICAST && rt2->rt_type != RTN_LOCAL)) {
			skb_dst_drop(skb);
			skb->_skb_refdst = orefdst;
			return -EINVAL;
		}
		refdst_drop(orefdst);
		if (rt2->rt_type != RTN_LOCAL)
			break;
		 
		memcpy(&iph->daddr, &optptr[srrptr-1], 4);
		opt->is_changed = 1;
	}
	if (srrptr <= srrspace) {
		opt->srr_is_hit = 1;
		opt->is_changed = 1;
	}
	return 0;
}