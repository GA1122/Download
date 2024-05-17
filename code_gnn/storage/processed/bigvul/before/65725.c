nfs4svc_decode_compoundargs(struct svc_rqst *rqstp, __be32 *p, struct nfsd4_compoundargs *args)
{
	if (rqstp->rq_arg.head[0].iov_len % 4) {
		 
		dprintk("%s: compound not properly padded! (peeraddr=%pISc xid=0x%x)",
			__func__, svc_addr(rqstp), be32_to_cpu(rqstp->rq_xid));
		return 0;
	}
	args->p = p;
	args->end = rqstp->rq_arg.head[0].iov_base + rqstp->rq_arg.head[0].iov_len;
	args->pagelist = rqstp->rq_arg.pages;
	args->pagelen = rqstp->rq_arg.page_len;
	args->tmpp = NULL;
	args->to_free = NULL;
	args->ops = args->iops;
	args->rqstp = rqstp;

	return !nfsd4_decode_compound(args);
}