int nfs4_proc_destroy_session(struct nfs4_session *session,
		struct rpc_cred *cred)
{
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_DESTROY_SESSION],
		.rpc_argp = session,
		.rpc_cred = cred,
	};
	int status = 0;

	dprintk("--> nfs4_proc_destroy_session\n");

	 
	if (!test_and_clear_bit(NFS4_SESSION_ESTABLISHED, &session->session_state))
		return 0;

	status = rpc_call_sync(session->clp->cl_rpcclient, &msg, RPC_TASK_TIMEOUT);
	trace_nfs4_destroy_session(session->clp, status);

	if (status)
		dprintk("NFS: Got error %d from the server on DESTROY_SESSION. "
			"Session has been destroyed regardless...\n", status);

	dprintk("<-- nfs4_proc_destroy_session\n");
	return status;
}