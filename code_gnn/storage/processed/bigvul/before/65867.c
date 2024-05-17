nfssvc_encode_readres(struct svc_rqst *rqstp, __be32 *p,
					struct nfsd_readres *resp)
{
	p = encode_fattr(rqstp, p, &resp->fh, &resp->stat);
	*p++ = htonl(resp->count);
	xdr_ressize_check(rqstp, p);

	 
	rqstp->rq_res.page_len = resp->count;
	if (resp->count & 3) {
		 
		rqstp->rq_res.tail[0].iov_base = p;
		*p = 0;
		rqstp->rq_res.tail[0].iov_len = 4 - (resp->count&3);
	}
	return 1;
}