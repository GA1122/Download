int nfs4_proc_setclientid(struct nfs_client *clp, u32 program,
		unsigned short port, struct rpc_cred *cred,
		struct nfs4_setclientid_res *res)
{
	nfs4_verifier sc_verifier;
	struct nfs4_setclientid setclientid = {
		.sc_verifier = &sc_verifier,
		.sc_prog = program,
		.sc_clnt = clp,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_SETCLIENTID],
		.rpc_argp = &setclientid,
		.rpc_resp = res,
		.rpc_cred = cred,
	};
	struct rpc_task *task;
	struct rpc_task_setup task_setup_data = {
		.rpc_client = clp->cl_rpcclient,
		.rpc_message = &msg,
		.callback_ops = &nfs4_setclientid_ops,
		.callback_data = &setclientid,
		.flags = RPC_TASK_TIMEOUT,
	};
	int status;

	 
	nfs4_init_boot_verifier(clp, &sc_verifier);

	if (test_bit(NFS_CS_MIGRATION, &clp->cl_flags))
		status = nfs4_init_uniform_client_string(clp);
	else
		status = nfs4_init_nonuniform_client_string(clp);

	if (status)
		goto out;

	 
	setclientid.sc_netid_len =
				nfs4_init_callback_netid(clp,
						setclientid.sc_netid,
						sizeof(setclientid.sc_netid));
	setclientid.sc_uaddr_len = scnprintf(setclientid.sc_uaddr,
				sizeof(setclientid.sc_uaddr), "%s.%u.%u",
				clp->cl_ipaddr, port >> 8, port & 255);

	dprintk("NFS call  setclientid auth=%s, '%s'\n",
		clp->cl_rpcclient->cl_auth->au_ops->au_name,
		clp->cl_owner_id);
	task = rpc_run_task(&task_setup_data);
	if (IS_ERR(task)) {
		status = PTR_ERR(task);
		goto out;
	}
	status = task->tk_status;
	if (setclientid.sc_cred) {
		clp->cl_acceptor = rpcauth_stringify_acceptor(setclientid.sc_cred);
		put_rpccred(setclientid.sc_cred);
	}
	rpc_put_task(task);
out:
	trace_nfs4_setclientid(clp, status);
	dprintk("NFS reply setclientid: %d\n", status);
	return status;
}