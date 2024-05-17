static void skb_set_err_queue(struct sk_buff *skb)
{
	 
	skb->pkt_type = PACKET_OUTGOING;
	BUILD_BUG_ON(PACKET_OUTGOING == 0);
}
