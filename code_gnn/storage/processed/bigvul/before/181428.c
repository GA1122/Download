  nfsd_dispatch(struct svc_rqst *rqstp, __be32 *statp)
  {
 	struct svc_procedure	*proc;
 	kxdrproc_t		xdr;
 	__be32			nfserr;
 	__be32			*nfserrp;
 
 	dprintk("nfsd_dispatch: vers %d proc %d\n",
  				rqstp->rq_vers, rqstp->rq_proc);
  	proc = rqstp->rq_procinfo;
  
// 	if (nfs_request_too_big(rqstp, proc)) {
// 		dprintk("nfsd: NFSv%d argument too large\n", rqstp->rq_vers);
// 		*statp = rpc_garbage_args;
// 		return 1;
// 	}
  	 
 	rqstp->rq_cachetype = proc->pc_cachetype;
 	 
 	xdr = proc->pc_decode;
 	if (xdr && !xdr(rqstp, (__be32*)rqstp->rq_arg.head[0].iov_base,
 			rqstp->rq_argp)) {
 		dprintk("nfsd: failed to decode arguments!\n");
 		*statp = rpc_garbage_args;
 		return 1;
 	}
 
 	 
 	switch (nfsd_cache_lookup(rqstp)) {
 	case RC_DROPIT:
 		return 0;
 	case RC_REPLY:
 		return 1;
 	case RC_DOIT:;
 		 
 	}
 
 	 
 	nfserrp = rqstp->rq_res.head[0].iov_base
 		+ rqstp->rq_res.head[0].iov_len;
 	rqstp->rq_res.head[0].iov_len += sizeof(__be32);
 
 	 
 	nfserr = proc->pc_func(rqstp, rqstp->rq_argp, rqstp->rq_resp);
 	nfserr = map_new_errors(rqstp->rq_vers, nfserr);
 	if (nfserr == nfserr_dropit || test_bit(RQ_DROPME, &rqstp->rq_flags)) {
 		dprintk("nfsd: Dropping request; may be revisited later\n");
 		nfsd_cache_update(rqstp, RC_NOCACHE, NULL);
 		return 0;
 	}
 
 	if (rqstp->rq_proc != 0)
 		*nfserrp++ = nfserr;
 
 	 
 	if (!(nfserr && rqstp->rq_vers == 2)) {
 		xdr = proc->pc_encode;
 		if (xdr && !xdr(rqstp, nfserrp,
 				rqstp->rq_resp)) {
 			 
 			dprintk("nfsd: failed to encode result!\n");
 			nfsd_cache_update(rqstp, RC_NOCACHE, NULL);
 			*statp = rpc_system_err;
 			return 1;
 		}
 	}
 
 	 
 	nfsd_cache_update(rqstp, rqstp->rq_cachetype, statp + 1);
 	return 1;
 }