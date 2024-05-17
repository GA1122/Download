struct sctp_association *sctp_make_temp_asoc(const struct sctp_endpoint *ep,
					struct sctp_chunk *chunk,
					gfp_t gfp)
{
	struct sctp_association *asoc;
	struct sk_buff *skb;
	sctp_scope_t scope;
	struct sctp_af *af;

	 
	scope = sctp_scope(sctp_source(chunk));
	asoc = sctp_association_new(ep, ep->base.sk, scope, gfp);
	if (!asoc)
		goto nodata;
	asoc->temp = 1;
	skb = chunk->skb;
	 
	af = sctp_get_af_specific(ipver2af(ip_hdr(skb)->version));
	if (unlikely(!af))
		goto fail;
	af->from_skb(&asoc->c.peer_addr, skb, 1);
nodata:
	return asoc;

fail:
	sctp_association_free(asoc);
	return NULL;
}