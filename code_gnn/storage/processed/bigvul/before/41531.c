static struct sk_buff *inet_gso_segment(struct sk_buff *skb,
					netdev_features_t features)
{
	struct sk_buff *segs = ERR_PTR(-EINVAL);
	const struct net_offload *ops;
	unsigned int offset = 0;
	bool udpfrag, encap;
	struct iphdr *iph;
	int proto;
	int nhoff;
	int ihl;
	int id;

	if (unlikely(skb_shinfo(skb)->gso_type &
		     ~(SKB_GSO_TCPV4 |
		       SKB_GSO_UDP |
		       SKB_GSO_DODGY |
		       SKB_GSO_TCP_ECN |
		       SKB_GSO_GRE |
		       SKB_GSO_GRE_CSUM |
		       SKB_GSO_IPIP |
		       SKB_GSO_SIT |
		       SKB_GSO_TCPV6 |
		       SKB_GSO_UDP_TUNNEL |
		       SKB_GSO_UDP_TUNNEL_CSUM |
		       SKB_GSO_TUNNEL_REMCSUM |
		       0)))
		goto out;

	skb_reset_network_header(skb);
	nhoff = skb_network_header(skb) - skb_mac_header(skb);
	if (unlikely(!pskb_may_pull(skb, sizeof(*iph))))
		goto out;

	iph = ip_hdr(skb);
	ihl = iph->ihl * 4;
	if (ihl < sizeof(*iph))
		goto out;

	id = ntohs(iph->id);
	proto = iph->protocol;

	 
	if (unlikely(!pskb_may_pull(skb, ihl)))
		goto out;
	__skb_pull(skb, ihl);

	encap = SKB_GSO_CB(skb)->encap_level > 0;
	if (encap)
		features &= skb->dev->hw_enc_features;
	SKB_GSO_CB(skb)->encap_level += ihl;

	skb_reset_transport_header(skb);

	segs = ERR_PTR(-EPROTONOSUPPORT);

	if (skb->encapsulation &&
	    skb_shinfo(skb)->gso_type & (SKB_GSO_SIT|SKB_GSO_IPIP))
		udpfrag = proto == IPPROTO_UDP && encap;
	else
		udpfrag = proto == IPPROTO_UDP && !skb->encapsulation;

	ops = rcu_dereference(inet_offloads[proto]);
	if (likely(ops && ops->callbacks.gso_segment))
		segs = ops->callbacks.gso_segment(skb, features);

	if (IS_ERR_OR_NULL(segs))
		goto out;

	skb = segs;
	do {
		iph = (struct iphdr *)(skb_mac_header(skb) + nhoff);
		if (udpfrag) {
			iph->id = htons(id);
			iph->frag_off = htons(offset >> 3);
			if (skb->next)
				iph->frag_off |= htons(IP_MF);
			offset += skb->len - nhoff - ihl;
		} else {
			iph->id = htons(id++);
		}
		iph->tot_len = htons(skb->len - nhoff);
		ip_send_check(iph);
		if (encap)
			skb_reset_inner_headers(skb);
		skb->network_header = (u8 *)iph - skb->head;
	} while ((skb = skb->next));

out:
	return segs;
}