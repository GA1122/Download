static struct sk_buff *gre_gso_segment(struct sk_buff *skb,
				       netdev_features_t features)
{
	int tnl_hlen = skb_inner_mac_header(skb) - skb_transport_header(skb);
	struct sk_buff *segs = ERR_PTR(-EINVAL);
	u16 mac_offset = skb->mac_header;
	__be16 protocol = skb->protocol;
	u16 mac_len = skb->mac_len;
	int gre_offset, outer_hlen;
	bool need_csum, ufo;

	if (unlikely(skb_shinfo(skb)->gso_type &
				~(SKB_GSO_TCPV4 |
				  SKB_GSO_TCPV6 |
				  SKB_GSO_UDP |
				  SKB_GSO_DODGY |
				  SKB_GSO_TCP_ECN |
				  SKB_GSO_GRE |
				  SKB_GSO_GRE_CSUM |
				  SKB_GSO_IPIP |
				  SKB_GSO_SIT)))
		goto out;

	if (!skb->encapsulation)
		goto out;

	if (unlikely(tnl_hlen < sizeof(struct gre_base_hdr)))
		goto out;

	if (unlikely(!pskb_may_pull(skb, tnl_hlen)))
		goto out;

	 
	skb->encapsulation = 0;
	__skb_pull(skb, tnl_hlen);
	skb_reset_mac_header(skb);
	skb_set_network_header(skb, skb_inner_network_offset(skb));
	skb->mac_len = skb_inner_network_offset(skb);
	skb->protocol = skb->inner_protocol;

	need_csum = !!(skb_shinfo(skb)->gso_type & SKB_GSO_GRE_CSUM);
	skb->encap_hdr_csum = need_csum;

	ufo = !!(skb_shinfo(skb)->gso_type & SKB_GSO_UDP);

	features &= skb->dev->hw_enc_features;

	 
	if (ufo) {
		features &= ~NETIF_F_CSUM_MASK;
		if (!need_csum)
			features |= NETIF_F_HW_CSUM;
	}

	 
	segs = skb_mac_gso_segment(skb, features);
	if (IS_ERR_OR_NULL(segs)) {
		skb_gso_error_unwind(skb, protocol, tnl_hlen, mac_offset,
				     mac_len);
		goto out;
	}

	outer_hlen = skb_tnl_header_len(skb);
	gre_offset = outer_hlen - tnl_hlen;
	skb = segs;
	do {
		struct gre_base_hdr *greh;
		__be32 *pcsum;

		 
		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			skb_reset_inner_headers(skb);
			skb->encapsulation = 1;
		}

		skb->mac_len = mac_len;
		skb->protocol = protocol;

		__skb_push(skb, outer_hlen);
		skb_reset_mac_header(skb);
		skb_set_network_header(skb, mac_len);
		skb_set_transport_header(skb, gre_offset);

		if (!need_csum)
			continue;

		greh = (struct gre_base_hdr *)skb_transport_header(skb);
		pcsum = (__be32 *)(greh + 1);

		*pcsum = 0;
		*(__sum16 *)pcsum = gso_make_checksum(skb, 0);
	} while ((skb = skb->next));
out:
	return segs;
}