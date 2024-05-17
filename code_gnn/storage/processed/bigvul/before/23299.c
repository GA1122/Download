static int decode_exchange_id(struct xdr_stream *xdr,
			      struct nfs41_exchange_id_res *res)
{
	__be32 *p;
	uint32_t dummy;
	char *dummy_str;
	int status;
	struct nfs_client *clp = res->client;

	status = decode_op_hdr(xdr, OP_EXCHANGE_ID);
	if (status)
		return status;

	p = xdr_inline_decode(xdr, 8);
	if (unlikely(!p))
		goto out_overflow;
	xdr_decode_hyper(p, &clp->cl_clientid);
	p = xdr_inline_decode(xdr, 12);
	if (unlikely(!p))
		goto out_overflow;
	clp->cl_seqid = be32_to_cpup(p++);
	clp->cl_exchange_flags = be32_to_cpup(p++);

	 
	dummy = be32_to_cpup(p);
	if (dummy != SP4_NONE)
		return -EIO;

	 
	p = xdr_inline_decode(xdr, 8);
	if (unlikely(!p))
		goto out_overflow;

	 
	status = decode_opaque_inline(xdr, &dummy, &dummy_str);
	if (unlikely(status))
		return status;

	 
	status = decode_opaque_inline(xdr, &dummy, &dummy_str);
	if (unlikely(status))
		return status;

	if (unlikely(dummy > NFS4_OPAQUE_LIMIT))
		return -EIO;

	memcpy(res->server_scope->server_scope, dummy_str, dummy);
	res->server_scope->server_scope_sz = dummy;

	 
	status = decode_opaque_inline(xdr, &dummy, &dummy_str);
	if (unlikely(status))
		return status;

	return 0;
out_overflow:
	print_overflow_msg(__func__, xdr);
	return -EIO;
}