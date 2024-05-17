static int nfs4_xdr_dec_rename(struct rpc_rqst *rqstp, struct xdr_stream *xdr,
			       struct nfs_renameres *res)
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
	status = decode_rename(xdr, &res->old_cinfo, &res->new_cinfo);
	if (status)
		goto out;
	 
	if (decode_getfattr(xdr, res->new_fattr, res->server,
				!RPC_IS_ASYNC(rqstp->rq_task)) != 0)
		goto out;
	status = decode_restorefh(xdr);
	if (status)
		goto out;
	decode_getfattr(xdr, res->old_fattr, res->server,
			!RPC_IS_ASYNC(rqstp->rq_task));
out:
	return status;
}