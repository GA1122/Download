static int atalk_route_packet(struct sk_buff *skb, struct net_device *dev,
			      struct ddpehdr *ddp, __u16 len_hops, int origlen)
{
	struct atalk_route *rt;
	struct atalk_addr ta;

	 
	if (skb->pkt_type != PACKET_HOST || !ddp->deh_dnet) {
		 
		if (dev->type == ARPHRD_PPP)
			printk(KERN_DEBUG "AppleTalk: didn't forward broadcast "
					  "packet received from PPP iface\n");
		goto free_it;
	}

	ta.s_net  = ddp->deh_dnet;
	ta.s_node = ddp->deh_dnode;

	 
	rt = atrtr_find(&ta);
	 
	len_hops += 1 << 10;
	if (!rt || !(len_hops & (15 << 10)))
		goto free_it;

	 

	 

	if (rt->flags & RTF_GATEWAY) {
		ta.s_net  = rt->gateway.s_net;
		ta.s_node = rt->gateway.s_node;
	}

	 
	skb_trim(skb, min_t(unsigned int, origlen,
			    (rt->dev->hard_header_len +
			     ddp_dl->header_length + (len_hops & 1023))));

	 
	ddp->deh_len_hops = htons(len_hops);

	 
	if (skb_headroom(skb) < 22) {
		 
		struct sk_buff *nskb = skb_realloc_headroom(skb, 32);
		kfree_skb(skb);
		skb = nskb;
	} else
		skb = skb_unshare(skb, GFP_ATOMIC);

	 
	if (skb == NULL)
		goto drop;

	if (aarp_send_ddp(rt->dev, skb, &ta, NULL) == NET_XMIT_DROP)
		return NET_RX_DROP;
	return NET_RX_SUCCESS;
free_it:
	kfree_skb(skb);
drop:
	return NET_RX_DROP;
}
