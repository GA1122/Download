static int nfs4_xdr_dec_link(struct rpc_rqst *rqstp, struct xdr_stream *xdr,
			     struct nfs4_link_res *res)
{
	struct compound_hdr hdr;
	int status;

	status = decode_compound_hdr(xdr, &hdr);
	if (status)
		goto out;
	status = decode_sequence(xdr, &res->seq_res, rqstp);
	if (status)
		goto out;
	status = decode_putfh(xdr);
	if (status)
		goto out;
	status = decode_savefh(xdr);
	if (status)
		goto out;
	status = decode_putfh(xdr);
	if (status)
		goto out;
	status = decode_link(xdr, &res->cinfo);
	if (status)
		goto out;
	 
	if (decode_getfattr(xdr, res->dir_attr, res->server,
				!RPC_IS_ASYNC(rqstp->rq_task)) != 0)
		goto out;
	status = decode_restorefh(xdr);
	if (status)
		goto out;
	decode_getfattr(xdr, res->fattr, res->server,
			!RPC_IS_ASYNC(rqstp->rq_task));
out:
	return status;
}