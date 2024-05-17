static void qdisc_pkt_len_init(struct sk_buff *skb)
{
	const struct skb_shared_info *shinfo = skb_shinfo(skb);

	qdisc_skb_cb(skb)->pkt_len = skb->len;

	 
	if (shinfo->gso_size)  {
		unsigned int hdr_len;
		u16 gso_segs = shinfo->gso_segs;

		 
		hdr_len = skb_transport_header(skb) - skb_mac_header(skb);

		 
		if (likely(shinfo->gso_type & (SKB_GSO_TCPV4 | SKB_GSO_TCPV6)))
			hdr_len += tcp_hdrlen(skb);
		else
			hdr_len += sizeof(struct udphdr);

		if (shinfo->gso_type & SKB_GSO_DODGY)
			gso_segs = DIV_ROUND_UP(skb->len - hdr_len,
						shinfo->gso_size);

		qdisc_skb_cb(skb)->pkt_len += (gso_segs - 1) * hdr_len;
	}
}