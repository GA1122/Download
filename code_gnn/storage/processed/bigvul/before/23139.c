static int nfs4_xdr_enc_lock(struct rpc_rqst *req, __be32 *p, struct nfs_lock_args *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops   = 2,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if(status)
		goto out;
	status = encode_lock(&xdr, args);
out:
	return status;
}