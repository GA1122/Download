 static struct sk_buff *udp6_ufo_fragment(struct sk_buff *skb,
 					 netdev_features_t features)
 {
 	struct sk_buff *segs = ERR_PTR(-EINVAL);
 	unsigned int mss;
 	unsigned int unfrag_ip6hlen, unfrag_len;
 	struct frag_hdr *fptr;
 	u8 *packet_start, *prevhdr;
 	u8 nexthdr;
 	u8 frag_hdr_sz = sizeof(struct frag_hdr);
 	__wsum csum;
 	int tnl_hlen;
 
 	mss = skb_shinfo(skb)->gso_size;
 	if (unlikely(skb->len <= mss))
 		goto out;
 
 	if (skb_gso_ok(skb, features | NETIF_F_GSO_ROBUST)) {
 		 
 
 		skb_shinfo(skb)->gso_segs = DIV_ROUND_UP(skb->len, mss);
 
 		 
 		if (!skb_shinfo(skb)->ip6_frag_id)
 			ipv6_proxy_select_ident(dev_net(skb->dev), skb);
 
 		segs = NULL;
 		goto out;
 	}
 
 	if (skb->encapsulation && skb_shinfo(skb)->gso_type &
 	    (SKB_GSO_UDP_TUNNEL|SKB_GSO_UDP_TUNNEL_CSUM))
 		segs = skb_udp_tunnel_segment(skb, features, true);
 	else {
 		const struct ipv6hdr *ipv6h;
 		struct udphdr *uh;
 
 		if (!pskb_may_pull(skb, sizeof(struct udphdr)))
 			goto out;
 
 		 
 
 		uh = udp_hdr(skb);
 		ipv6h = ipv6_hdr(skb);
 
 		uh->check = 0;
 		csum = skb_checksum(skb, 0, skb->len, 0);
 		uh->check = udp_v6_check(skb->len, &ipv6h->saddr,
 					  &ipv6h->daddr, csum);
 		if (uh->check == 0)
 			uh->check = CSUM_MANGLED_0;
 
 		skb->ip_summed = CHECKSUM_NONE;
 
 		 
 		if (!skb->encap_hdr_csum)
 			features |= NETIF_F_HW_CSUM;
 
 		 
 		tnl_hlen = skb_tnl_header_len(skb);
 		if (skb->mac_header < (tnl_hlen + frag_hdr_sz)) {
 			if (gso_pskb_expand_head(skb, tnl_hlen + frag_hdr_sz))
 				goto out;
 		}
 
 		 
  		unfrag_ip6hlen = ip6_find_1stfragopt(skb, &prevhdr);
// 		if (unfrag_ip6hlen < 0)
// 			return ERR_PTR(unfrag_ip6hlen);
  		nexthdr = *prevhdr;
  		*prevhdr = NEXTHDR_FRAGMENT;
  		unfrag_len = (skb_network_header(skb) - skb_mac_header(skb)) +
 			     unfrag_ip6hlen + tnl_hlen;
 		packet_start = (u8 *) skb->head + SKB_GSO_CB(skb)->mac_offset;
 		memmove(packet_start-frag_hdr_sz, packet_start, unfrag_len);
 
 		SKB_GSO_CB(skb)->mac_offset -= frag_hdr_sz;
 		skb->mac_header -= frag_hdr_sz;
 		skb->network_header -= frag_hdr_sz;
 
 		fptr = (struct frag_hdr *)(skb_network_header(skb) + unfrag_ip6hlen);
 		fptr->nexthdr = nexthdr;
 		fptr->reserved = 0;
 		if (!skb_shinfo(skb)->ip6_frag_id)
 			ipv6_proxy_select_ident(dev_net(skb->dev), skb);
 		fptr->identification = skb_shinfo(skb)->ip6_frag_id;
 
 		 
 		segs = skb_segment(skb, features);
 	}
 
 out:
 	return segs;
 }