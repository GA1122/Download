rpc_encode_header(struct rpc_task *task)
{
	struct rpc_clnt *clnt = task->tk_client;
	struct rpc_rqst	*req = task->tk_rqstp;
	__be32		*p = req->rq_svec[0].iov_base;

	 

	p = xprt_skip_transport_header(task->tk_xprt, p);
	*p++ = req->rq_xid;		 
	*p++ = htonl(RPC_CALL);		 
	*p++ = htonl(RPC_VERSION);	 
	*p++ = htonl(clnt->cl_prog);	 
	*p++ = htonl(clnt->cl_vers);	 
	*p++ = htonl(task->tk_msg.rpc_proc->p_proc);	 
	p = rpcauth_marshcred(task, p);
	req->rq_slen = xdr_adjust_iovec(&req->rq_svec[0], p);
	return p;
}
