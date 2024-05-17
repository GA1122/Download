netdev_features_t netif_skb_features(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	netdev_features_t features = dev->features;
	u16 gso_segs = skb_shinfo(skb)->gso_segs;

	if (gso_segs > dev->gso_max_segs || gso_segs < dev->gso_min_segs)
		features &= ~NETIF_F_GSO_MASK;

	 
	if (skb->encapsulation)
		features &= dev->hw_enc_features;

	if (skb_vlan_tagged(skb))
		features = netdev_intersect_features(features,
						     dev->vlan_features |
						     NETIF_F_HW_VLAN_CTAG_TX |
						     NETIF_F_HW_VLAN_STAG_TX);

	if (dev->netdev_ops->ndo_features_check)
		features &= dev->netdev_ops->ndo_features_check(skb, dev,
								features);
	else
		features &= dflt_features_check(skb, dev, features);

	return harmonize_features(skb, features);
}