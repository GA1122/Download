bool is_skb_forwardable(struct net_device *dev, struct sk_buff *skb)
{
	unsigned int len;

	if (!(dev->flags & IFF_UP))
		return false;

	len = dev->mtu + dev->hard_header_len + VLAN_HLEN;
	if (skb->len <= len)
		return true;

	 
	if (skb_is_gso(skb))
		return true;

	return false;
}