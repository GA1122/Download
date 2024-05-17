static int pptp_rcv(struct sk_buff *skb)
{
	struct pppox_sock *po;
	struct pptp_gre_header *header;
	struct iphdr *iph;

	if (skb->pkt_type != PACKET_HOST)
		goto drop;

	if (!pskb_may_pull(skb, 12))
		goto drop;

	iph = ip_hdr(skb);

	header = (struct pptp_gre_header *)skb->data;

	if (ntohs(header->protocol) != PPTP_GRE_PROTO ||  
		PPTP_GRE_IS_C(header->flags) ||                 
		PPTP_GRE_IS_R(header->flags) ||                 
		!PPTP_GRE_IS_K(header->flags) ||                
		(header->flags&0xF) != 0)                       
		 
		goto drop;

	po = lookup_chan(htons(header->call_id), iph->saddr);
	if (po) {
		skb_dst_drop(skb);
		nf_reset(skb);
		return sk_receive_skb(sk_pppox(po), skb, 0);
	}
drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}