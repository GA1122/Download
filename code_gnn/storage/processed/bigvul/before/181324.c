 nfssvc_decode_readlinkargs(struct svc_rqst *rqstp, __be32 *p, struct nfsd_readlinkargs *args)
 {
  	p = decode_fh(p, &args->fh);
  	if (!p)
  		return 0;
// 	if (!xdr_argsize_check(rqstp, p))
// 		return 0;
  	args->buffer = page_address(*(rqstp->rq_next_page++));
  
	return xdr_argsize_check(rqstp, p);
// 	return 1;
  }