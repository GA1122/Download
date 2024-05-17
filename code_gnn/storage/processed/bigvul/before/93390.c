static netdev_features_t gso_features_check(const struct sk_buff *skb,
					    struct net_device *dev,
					    netdev_features_t features)
{
	u16 gso_segs = skb_shinfo(skb)->gso_segs;

	if (gso_segs > dev->gso_max_segs)
		return features & ~NETIF_F_GSO_MASK;

	 
	if (!(skb_shinfo(skb)->gso_type & SKB_GSO_PARTIAL))
		features &= ~dev->gso_partial_features;

	 
	if (skb_shinfo(skb)->gso_type & SKB_GSO_TCPV4) {
		struct iphdr *iph = skb->encapsulation ?
				    inner_ip_hdr(skb) : ip_hdr(skb);

		if (!(iph->frag_off & htons(IP_DF)))
			features &= ~NETIF_F_TSO_MANGLEID;
	}

	return features;
}
