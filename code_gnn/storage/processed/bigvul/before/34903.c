rpc_free_auth(struct rpc_clnt *clnt)
{
	if (clnt->cl_auth == NULL) {
		rpc_free_client(clnt);
		return;
	}

	 
	atomic_inc(&clnt->cl_count);
	rpcauth_release(clnt->cl_auth);
	clnt->cl_auth = NULL;
	if (atomic_dec_and_test(&clnt->cl_count))
		rpc_free_client(clnt);
}