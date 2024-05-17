 svc_rdma_is_backchannel_reply(struct svc_xprt *xprt, struct rpcrdma_msg *rmsgp)
// static bool svc_rdma_is_backchannel_reply(struct svc_xprt *xprt,
// 					  __be32 *rdma_resp)
  {
	__be32 *p = (__be32 *)rmsgp;
// 	__be32 *p;
  
  	if (!xprt->xpt_bc_xprt)
  		return false;
  
	if (rmsgp->rm_type != rdma_msg)
// 	p = rdma_resp + 3;
// 	if (*p++ != rdma_msg)
  		return false;
	if (rmsgp->rm_body.rm_chunks[0] != xdr_zero)
// 
// 	if (*p++ != xdr_zero)
  		return false;
	if (rmsgp->rm_body.rm_chunks[1] != xdr_zero)
// 	if (*p++ != xdr_zero)
  		return false;
	if (rmsgp->rm_body.rm_chunks[2] != xdr_zero)
// 	if (*p++ != xdr_zero)
  		return false;
  
 	 
	if (p[7] != rmsgp->rm_xid)
// 	if (*p++ != *rdma_resp)
  		return false;
  	 
	if (p[8] == cpu_to_be32(RPC_CALL))
// 	if (*p == cpu_to_be32(RPC_CALL))
  		return false;
  
  	return true;
 }