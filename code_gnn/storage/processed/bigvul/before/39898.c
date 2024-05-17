static void skb_headers_offset_update(struct sk_buff *skb, int off)
{
	 
	if (skb->ip_summed == CHECKSUM_PARTIAL)
		skb->csum_start += off;
	 
	skb->transport_header += off;
	skb->network_header   += off;
	if (skb_mac_header_was_set(skb))
		skb->mac_header += off;
	skb->inner_transport_header += off;
	skb->inner_network_header += off;
	skb->inner_mac_header += off;
}
