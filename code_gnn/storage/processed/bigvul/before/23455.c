static void nfs4_xdr_enc_commit(struct rpc_rqst *req, struct xdr_stream *xdr,
				struct nfs_writeargs *args)
{
	struct compound_hdr hdr = {
		.minorversion = nfs4_xdr_minorversion(&args->seq_args),
	};

	encode_compound_hdr(xdr, req, &hdr);
	encode_sequence(xdr, &args->seq_args, &hdr);
	encode_putfh(xdr, args->fh, &hdr);
	encode_commit(xdr, args, &hdr);
	if (args->bitmask)
		encode_getfattr(xdr, args->bitmask, &hdr);
	encode_nops(&hdr);
}