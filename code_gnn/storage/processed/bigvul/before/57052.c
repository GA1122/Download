static int _nfs4_proc_exchange_id(struct nfs_client *clp, struct rpc_cred *cred,
	u32 sp4_how)
{
	nfs4_verifier verifier;
	struct nfs41_exchange_id_args args = {
		.verifier = &verifier,
		.client = clp,
#ifdef CONFIG_NFS_V4_1_MIGRATION
		.flags = EXCHGID4_FLAG_SUPP_MOVED_REFER |
			 EXCHGID4_FLAG_BIND_PRINC_STATEID |
			 EXCHGID4_FLAG_SUPP_MOVED_MIGR,
#else
		.flags = EXCHGID4_FLAG_SUPP_MOVED_REFER |
			 EXCHGID4_FLAG_BIND_PRINC_STATEID,
#endif
	};
	struct nfs41_exchange_id_res res = {
		0
	};
	int status;
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_EXCHANGE_ID],
		.rpc_argp = &args,
		.rpc_resp = &res,
		.rpc_cred = cred,
	};

	nfs4_init_boot_verifier(clp, &verifier);

	status = nfs4_init_uniform_client_string(clp);
	if (status)
		goto out;

	dprintk("NFS call  exchange_id auth=%s, '%s'\n",
		clp->cl_rpcclient->cl_auth->au_ops->au_name,
		clp->cl_owner_id);

	res.server_owner = kzalloc(sizeof(struct nfs41_server_owner),
					GFP_NOFS);
	if (unlikely(res.server_owner == NULL)) {
		status = -ENOMEM;
		goto out;
	}

	res.server_scope = kzalloc(sizeof(struct nfs41_server_scope),
					GFP_NOFS);
	if (unlikely(res.server_scope == NULL)) {
		status = -ENOMEM;
		goto out_server_owner;
	}

	res.impl_id = kzalloc(sizeof(struct nfs41_impl_id), GFP_NOFS);
	if (unlikely(res.impl_id == NULL)) {
		status = -ENOMEM;
		goto out_server_scope;
	}

	switch (sp4_how) {
	case SP4_NONE:
		args.state_protect.how = SP4_NONE;
		break;

	case SP4_MACH_CRED:
		args.state_protect = nfs4_sp4_mach_cred_request;
		break;

	default:
		 
		WARN_ON_ONCE(1);
		status = -EINVAL;
		goto out_impl_id;
	}

	status = rpc_call_sync(clp->cl_rpcclient, &msg, RPC_TASK_TIMEOUT);
	trace_nfs4_exchange_id(clp, status);
	if (status == 0)
		status = nfs4_check_cl_exchange_flags(res.flags);

	if (status == 0)
		status = nfs4_sp4_select_mode(clp, &res.state_protect);

	if (status == 0) {
		clp->cl_clientid = res.clientid;
		clp->cl_exchange_flags = res.flags;
		 
		if (!(res.flags & EXCHGID4_FLAG_CONFIRMED_R)) {
			clear_bit(NFS4_SESSION_ESTABLISHED,
					&clp->cl_session->session_state);
			clp->cl_seqid = res.seqid;
		}

		kfree(clp->cl_serverowner);
		clp->cl_serverowner = res.server_owner;
		res.server_owner = NULL;

		 
		kfree(clp->cl_implid);
		clp->cl_implid = res.impl_id;
		res.impl_id = NULL;

		if (clp->cl_serverscope != NULL &&
		    !nfs41_same_server_scope(clp->cl_serverscope,
					     res.server_scope)) {
			dprintk("%s: server_scope mismatch detected\n",
				__func__);
			set_bit(NFS4CLNT_SERVER_SCOPE_MISMATCH, &clp->cl_state);
			kfree(clp->cl_serverscope);
			clp->cl_serverscope = NULL;
		}

		if (clp->cl_serverscope == NULL) {
			clp->cl_serverscope = res.server_scope;
			res.server_scope = NULL;
		}
	}

out_impl_id:
	kfree(res.impl_id);
out_server_scope:
	kfree(res.server_scope);
out_server_owner:
	kfree(res.server_owner);
out:
	if (clp->cl_implid != NULL)
		dprintk("NFS reply exchange_id: Server Implementation ID: "
			"domain: %s, name: %s, date: %llu,%u\n",
			clp->cl_implid->domain, clp->cl_implid->name,
			clp->cl_implid->date.seconds,
			clp->cl_implid->date.nseconds);
	dprintk("NFS reply exchange_id: %d\n", status);
	return status;
}