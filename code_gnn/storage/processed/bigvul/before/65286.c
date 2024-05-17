nfs3svc_encode_fsstatres(struct svc_rqst *rqstp, __be32 *p,
					struct nfsd3_fsstatres *resp)
{
	struct kstatfs	*s = &resp->stats;
	u64		bs = s->f_bsize;

	*p++ = xdr_zero;	 

	if (resp->status == 0) {
		p = xdr_encode_hyper(p, bs * s->f_blocks);	 
		p = xdr_encode_hyper(p, bs * s->f_bfree);	 
		p = xdr_encode_hyper(p, bs * s->f_bavail);	 
		p = xdr_encode_hyper(p, s->f_files);	 
		p = xdr_encode_hyper(p, s->f_ffree);	 
		p = xdr_encode_hyper(p, s->f_ffree);	 
		*p++ = htonl(resp->invarsec);	 
	}
	return xdr_ressize_check(rqstp, p);
}
