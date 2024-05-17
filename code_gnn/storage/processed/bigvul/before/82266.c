static bool skb_is_err_queue(const struct sk_buff *skb)
{
	 
	return skb->pkt_type == PACKET_OUTGOING;
}
