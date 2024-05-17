 static void svc_rdma_get_write_arrays(struct rpcrdma_msg *rmsgp,
				      struct rpcrdma_write_array **write,
				      struct rpcrdma_write_array **reply)
// static void svc_rdma_get_write_arrays(__be32 *rdma_argp,
// 				      __be32 **write, __be32 **reply)
  {
  	__be32 *p;
  
	p = (__be32 *)&rmsgp->rm_body.rm_chunks[0];
// 	p = rdma_argp + rpcrdma_fixed_maxsz;
  
  	 
  	while (*p++ != xdr_zero)
  		p += 5;
  
  	 
  	if (*p != xdr_zero) {
		*write = (struct rpcrdma_write_array *)p;
// 		*write = p;
  		while (*p++ != xdr_zero)
  			p += 1 + be32_to_cpu(*p) * 4;
  	} else {
 		*write = NULL;
 		p++;
 	}
  
  	 
  	if (*p != xdr_zero)
		*reply = (struct rpcrdma_write_array *)p;
// 		*reply = p;
  	else
  		*reply = NULL;
  }