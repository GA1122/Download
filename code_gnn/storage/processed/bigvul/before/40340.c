static int ltalk_rcv(struct sk_buff *skb, struct net_device *dev,
		     struct packet_type *pt, struct net_device *orig_dev)
{
	if (!net_eq(dev_net(dev), &init_net))
		goto freeit;

	 
	if (skb_mac_header(skb)[2] == 1) {
		struct ddpehdr *ddp;
		 
		struct atalk_addr *ap = atalk_find_dev_addr(dev);

		if (!ap || skb->len < sizeof(__be16) || skb->len > 1023)
			goto freeit;

		 
		if (!(skb = skb_share_check(skb, GFP_ATOMIC)))
			return 0;

		 
		ddp = (struct ddpehdr *) skb_push(skb, sizeof(*ddp) - 4);

		 

		 

		ddp->deh_dnode = skb_mac_header(skb)[0];      
		ddp->deh_snode = skb_mac_header(skb)[1];      

		ddp->deh_dnet  = ap->s_net;	 
		ddp->deh_snet  = ap->s_net;
		ddp->deh_sum   = 0;		 
		 
		 
		ddp->deh_len_hops = htons(skb->len + (DDP_MAXHOPS << 10));
	}
	skb_reset_transport_header(skb);

	return atalk_rcv(skb, dev, pt, orig_dev);
freeit:
	kfree_skb(skb);
	return 0;
}
