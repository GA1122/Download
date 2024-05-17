static void nfs4_xdr_enc_getdeviceinfo(struct rpc_rqst *req,
				       struct xdr_stream *xdr,
				       struct nfs4_getdeviceinfo_args *args)
{
	struct compound_hdr hdr = {
		.minorversion = nfs4_xdr_minorversion(&args->seq_args),
	};

	encode_compound_hdr(xdr, req, &hdr);
	encode_sequence(xdr, &args->seq_args, &hdr);
	encode_getdeviceinfo(xdr, args, &hdr);

	 
	xdr_inline_pages(&req->rq_rcv_buf, (hdr.replen - 2) << 2,
			 args->pdev->pages, args->pdev->pgbase,
			 args->pdev->pglen);

	encode_nops(&hdr);
}
