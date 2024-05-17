nfs3svc_decode_readdirplusargs(struct svc_rqst *rqstp, __be32 *p,
					struct nfsd3_readdirargs *args)
{
	int len;
	u32 max_blocksize = svc_max_payload(rqstp);

	p = decode_fh(p, &args->fh);
	if (!p)
		return 0;
	p = xdr_decode_hyper(p, &args->cookie);
	args->verf     = p; p += 2;
	args->dircount = ntohl(*p++);
	args->count    = ntohl(*p++);

	len = args->count = min(args->count, max_blocksize);
	while (len > 0) {
		struct page *p = *(rqstp->rq_next_page++);
		if (!args->buffer)
			args->buffer = page_address(p);
		len -= PAGE_SIZE;
	}

	return xdr_argsize_check(rqstp, p);
}