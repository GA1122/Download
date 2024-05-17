static int ipxitf_pprop(struct ipx_interface *intrfc, struct sk_buff *skb)
{
	struct ipxhdr *ipx = ipx_hdr(skb);
	int i, rc = -EINVAL;
	struct ipx_interface *ifcs;
	char *c;
	__be32 *l;

	 
	 
	if (IPX_SKB_CB(skb)->ipx_tctrl > IPX_MAX_PPROP_HOPS ||
	    ntohs(ipx->ipx_pktsize) < sizeof(struct ipxhdr) +
					IPX_MAX_PPROP_HOPS * sizeof(u32))
		goto out;
	 
	rc = 0;
	if (!sysctl_ipx_pprop_broadcasting)
		goto out;
	 
	if (IPX_SKB_CB(skb)->ipx_tctrl == IPX_MAX_PPROP_HOPS)
		goto out;

	c = ((u8 *) ipx) + sizeof(struct ipxhdr);
	l = (__be32 *) c;

	 
	for (i = 0; i < IPX_SKB_CB(skb)->ipx_tctrl; i++)
		if (*l++ == intrfc->if_netnum)
			goto out;

	 
	IPX_SKB_CB(skb)->last_hop.index = i;
	IPX_SKB_CB(skb)->last_hop.netnum = intrfc->if_netnum;
	 
	spin_lock_bh(&ipx_interfaces_lock);
	list_for_each_entry(ifcs, &ipx_interfaces, node) {
		 
		if (!ifcs->if_netnum)
			continue;

		 
		if (ifcs == intrfc)
			continue;
		l = (__be32 *) c;
		 
		for (i = 0; i < IPX_SKB_CB(skb)->ipx_tctrl; i++)
			if (ifcs->if_netnum == *l++)
				break;
		if (i == IPX_SKB_CB(skb)->ipx_tctrl) {
			struct sk_buff *s = skb_copy(skb, GFP_ATOMIC);

			if (s) {
				IPX_SKB_CB(s)->ipx_dest_net = ifcs->if_netnum;
				ipxrtr_route_skb(s);
			}
		}
	}
	spin_unlock_bh(&ipx_interfaces_lock);
out:
	return rc;
}
