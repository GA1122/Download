ppp_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ppp *ppp = netdev_priv(dev);
	int npi, proto;
	unsigned char *pp;

	npi = ethertype_to_npindex(ntohs(skb->protocol));
	if (npi < 0)
		goto outf;

	 
	switch (ppp->npmode[npi]) {
	case NPMODE_PASS:
		break;
	case NPMODE_QUEUE:
		 
		goto outf;
	case NPMODE_DROP:
	case NPMODE_ERROR:
		goto outf;
	}

	 
	if (skb_cow_head(skb, PPP_HDRLEN))
		goto outf;

	pp = skb_push(skb, 2);
	proto = npindex_to_proto[npi];
	put_unaligned_be16(proto, pp);

	skb_scrub_packet(skb, !net_eq(ppp->ppp_net, dev_net(dev)));
	skb_queue_tail(&ppp->file.xq, skb);
	ppp_xmit_process(ppp);
	return NETDEV_TX_OK;

 outf:
	kfree_skb(skb);
	++dev->stats.tx_dropped;
	return NETDEV_TX_OK;
}
