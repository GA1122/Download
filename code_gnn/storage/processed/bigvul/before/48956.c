static struct sk_buff *validate_xmit_skb(struct sk_buff *skb, struct net_device *dev)
{
	netdev_features_t features;

	if (skb->next)
		return skb;

	features = netif_skb_features(skb);
	skb = validate_xmit_vlan(skb, features);
	if (unlikely(!skb))
		goto out_null;

	if (netif_needs_gso(skb, features)) {
		struct sk_buff *segs;

		segs = skb_gso_segment(skb, features);
		if (IS_ERR(segs)) {
			goto out_kfree_skb;
		} else if (segs) {
			consume_skb(skb);
			skb = segs;
		}
	} else {
		if (skb_needs_linearize(skb, features) &&
		    __skb_linearize(skb))
			goto out_kfree_skb;

		 
		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			if (skb->encapsulation)
				skb_set_inner_transport_header(skb,
							       skb_checksum_start_offset(skb));
			else
				skb_set_transport_header(skb,
							 skb_checksum_start_offset(skb));
			if (!(features & NETIF_F_CSUM_MASK) &&
			    skb_checksum_help(skb))
				goto out_kfree_skb;
		}
	}

	return skb;

out_kfree_skb:
	kfree_skb(skb);
out_null:
	return NULL;
}
