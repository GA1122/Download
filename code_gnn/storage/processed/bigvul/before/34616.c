static struct sctp_association *__sctp_rcv_lookup_harder(struct sk_buff *skb,
				      const union sctp_addr *laddr,
				      struct sctp_transport **transportp)
{
	sctp_chunkhdr_t *ch;

	ch = (sctp_chunkhdr_t *) skb->data;

	 
	if (WORD_ROUND(ntohs(ch->length)) > skb->len)
		return NULL;

	 
	switch (ch->type) {
	case SCTP_CID_INIT:
	case SCTP_CID_INIT_ACK:
		return __sctp_rcv_init_lookup(skb, laddr, transportp);
		break;

	default:
		return __sctp_rcv_walk_lookup(skb, laddr, transportp);
		break;
	}


	return NULL;
}
