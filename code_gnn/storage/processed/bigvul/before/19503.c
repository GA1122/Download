static void tso_start(struct tso_state *st, const struct sk_buff *skb)
{
	 
	st->header_len = ((tcp_hdr(skb)->doff << 2u)
			  + PTR_DIFF(tcp_hdr(skb), skb->data));
	st->full_packet_size = st->header_len + skb_shinfo(skb)->gso_size;

	if (st->protocol == htons(ETH_P_IP))
		st->ipv4_id = ntohs(ip_hdr(skb)->id);
	else
		st->ipv4_id = 0;
	st->seqnum = ntohl(tcp_hdr(skb)->seq);

	EFX_BUG_ON_PARANOID(tcp_hdr(skb)->urg);
	EFX_BUG_ON_PARANOID(tcp_hdr(skb)->syn);
	EFX_BUG_ON_PARANOID(tcp_hdr(skb)->rst);

	st->packet_space = st->full_packet_size;
	st->out_len = skb->len - st->header_len;
	st->unmap_len = 0;
	st->unmap_single = false;
}
