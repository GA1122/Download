static int nfs4_xdr_dec_readdir(struct rpc_rqst *rqstp, __be32 *p, struct nfs4_readdir_res *res)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (status)
		goto out;
	status = decode_putfh(&xdr);
	if (status)
		goto out;
	status = decode_readdir(&xdr, rqstp, res);
out:
	return status;
}