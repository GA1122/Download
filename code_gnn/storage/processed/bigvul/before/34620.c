static void sctp_add_backlog(struct sock *sk, struct sk_buff *skb)
{
	struct sctp_chunk *chunk = SCTP_INPUT_CB(skb)->chunk;
	struct sctp_ep_common *rcvr = chunk->rcvr;

	 
	if (SCTP_EP_TYPE_ASSOCIATION == rcvr->type)
		sctp_association_hold(sctp_assoc(rcvr));
	else if (SCTP_EP_TYPE_SOCKET == rcvr->type)
		sctp_endpoint_hold(sctp_ep(rcvr));
	else
		BUG();

	sk_add_backlog(sk, skb);
}
