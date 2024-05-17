static struct sctp_association *__sctp_rcv_init_lookup(struct sk_buff *skb,
	const union sctp_addr *laddr, struct sctp_transport **transportp)
{
	struct sctp_association *asoc;
	union sctp_addr addr;
	union sctp_addr *paddr = &addr;
	struct sctphdr *sh = sctp_hdr(skb);
	sctp_chunkhdr_t *ch;
	union sctp_params params;
	sctp_init_chunk_t *init;
	struct sctp_transport *transport;
	struct sctp_af *af;

	ch = (sctp_chunkhdr_t *) skb->data;

	 

	 
	init = (sctp_init_chunk_t *)skb->data;

	 
	sctp_walk_params(params, init, init_hdr.params) {

		 
		af = sctp_get_af_specific(param_type2af(params.p->type));
		if (!af)
			continue;

		af->from_addr_param(paddr, params.addr, sh->source, 0);

		asoc = __sctp_lookup_association(laddr, paddr, &transport);
		if (asoc)
			return asoc;
	}

	return NULL;
}
