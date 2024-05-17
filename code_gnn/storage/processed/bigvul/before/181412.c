 nfssvc_decode_writeargs(struct svc_rqst *rqstp, __be32 *p,
 					struct nfsd_writeargs *args)
 {
 	unsigned int len, hdr, dlen;
 	struct kvec *head = rqstp->rq_arg.head;
 	int v;
 
 	p = decode_fh(p, &args->fh);
 	if (!p)
 		return 0;
 
 	p++;				 
 	args->offset = ntohl(*p++);	 
 	p++;				 
 	len = args->len = ntohl(*p++);
 	 
 	if (len > NFSSVC_MAXBLKSIZE_V2)
 		return 0;
 
 	 
  	hdr = (void*)p - head->iov_base;
// 	if (hdr > head->iov_len)
// 		return 0;
  	dlen = head->iov_len + rqstp->rq_arg.page_len - hdr;
  
  	 
 	if (dlen < XDR_QUADLEN(len)*4)
 		return 0;
 
 	rqstp->rq_vec[0].iov_base = (void*)p;
 	rqstp->rq_vec[0].iov_len = head->iov_len - hdr;
 	v = 0;
 	while (len > rqstp->rq_vec[v].iov_len) {
 		len -= rqstp->rq_vec[v].iov_len;
 		v++;
 		rqstp->rq_vec[v].iov_base = page_address(rqstp->rq_pages[v]);
 		rqstp->rq_vec[v].iov_len = PAGE_SIZE;
 	}
 	rqstp->rq_vec[v].iov_len = len;
 	args->vlen = v + 1;
 	return 1;
 }