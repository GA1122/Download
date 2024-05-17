static void encode_exchange_id(struct xdr_stream *xdr,
			       struct nfs41_exchange_id_args *args,
			       struct compound_hdr *hdr)
{
	__be32 *p;

	p = reserve_space(xdr, 4 + sizeof(args->verifier->data));
	*p++ = cpu_to_be32(OP_EXCHANGE_ID);
	xdr_encode_opaque_fixed(p, args->verifier->data, sizeof(args->verifier->data));

	encode_string(xdr, args->id_len, args->id);

	p = reserve_space(xdr, 12);
	*p++ = cpu_to_be32(args->flags);
	*p++ = cpu_to_be32(0);	 
	*p = cpu_to_be32(0);	 
	hdr->nops++;
	hdr->replen += decode_exchange_id_maxsz;
}