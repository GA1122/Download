static struct sctp_association *__sctp_rcv_lookup(struct sk_buff *skb,
				      const union sctp_addr *paddr,
				      const union sctp_addr *laddr,
				      struct sctp_transport **transportp)
{
	struct sctp_association *asoc;

	asoc = __sctp_lookup_association(laddr, paddr, transportp);

	 
	if (!asoc)
		asoc = __sctp_rcv_lookup_harder(skb, laddr, transportp);

	return asoc;
}
