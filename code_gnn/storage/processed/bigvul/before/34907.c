static struct rpc_clnt * rpc_new_client(const struct rpc_create_args *args, struct rpc_xprt *xprt)
{
	struct rpc_program	*program = args->program;
	struct rpc_version	*version;
	struct rpc_clnt		*clnt = NULL;
	struct rpc_auth		*auth;
	int err;
	size_t len;

	 
	err = -EINVAL;
	len = strlen(args->servername);
	if (len > RPC_MAXNETNAMELEN)
		goto out_no_rpciod;
	len++;

	dprintk("RPC:       creating %s client for %s (xprt %p)\n",
			program->name, args->servername, xprt);

	err = rpciod_up();
	if (err)
		goto out_no_rpciod;
	err = -EINVAL;
	if (!xprt)
		goto out_no_xprt;

	if (args->version >= program->nrvers)
		goto out_err;
	version = program->version[args->version];
	if (version == NULL)
		goto out_err;

	err = -ENOMEM;
	clnt = kzalloc(sizeof(*clnt), GFP_KERNEL);
	if (!clnt)
		goto out_err;
	clnt->cl_parent = clnt;

	clnt->cl_server = clnt->cl_inline_name;
	if (len > sizeof(clnt->cl_inline_name)) {
		char *buf = kmalloc(len, GFP_KERNEL);
		if (buf != NULL)
			clnt->cl_server = buf;
		else
			len = sizeof(clnt->cl_inline_name);
	}
	strlcpy(clnt->cl_server, args->servername, len);

	clnt->cl_xprt     = xprt;
	clnt->cl_procinfo = version->procs;
	clnt->cl_maxproc  = version->nrprocs;
	clnt->cl_protname = program->name;
	clnt->cl_prog     = args->prognumber ? : program->number;
	clnt->cl_vers     = version->number;
	clnt->cl_stats    = program->stats;
	clnt->cl_metrics  = rpc_alloc_iostats(clnt);
	err = -ENOMEM;
	if (clnt->cl_metrics == NULL)
		goto out_no_stats;
	clnt->cl_program  = program;
	INIT_LIST_HEAD(&clnt->cl_tasks);
	spin_lock_init(&clnt->cl_lock);

	if (!xprt_bound(clnt->cl_xprt))
		clnt->cl_autobind = 1;

	clnt->cl_timeout = xprt->timeout;
	if (args->timeout != NULL) {
		memcpy(&clnt->cl_timeout_default, args->timeout,
				sizeof(clnt->cl_timeout_default));
		clnt->cl_timeout = &clnt->cl_timeout_default;
	}

	clnt->cl_rtt = &clnt->cl_rtt_default;
	rpc_init_rtt(&clnt->cl_rtt_default, clnt->cl_timeout->to_initval);
	clnt->cl_principal = NULL;
	if (args->client_name) {
		clnt->cl_principal = kstrdup(args->client_name, GFP_KERNEL);
		if (!clnt->cl_principal)
			goto out_no_principal;
	}

	atomic_set(&clnt->cl_count, 1);

	err = rpc_setup_pipedir(clnt, program->pipe_dir_name);
	if (err < 0)
		goto out_no_path;

	auth = rpcauth_create(args->authflavor, clnt);
	if (IS_ERR(auth)) {
		printk(KERN_INFO "RPC: Couldn't create auth handle (flavor %u)\n",
				args->authflavor);
		err = PTR_ERR(auth);
		goto out_no_auth;
	}

	 
	clnt->cl_nodelen = strlen(init_utsname()->nodename);
	if (clnt->cl_nodelen > UNX_MAXNODENAME)
		clnt->cl_nodelen = UNX_MAXNODENAME;
	memcpy(clnt->cl_nodename, init_utsname()->nodename, clnt->cl_nodelen);
	rpc_register_client(clnt);
	return clnt;

out_no_auth:
	if (!IS_ERR(clnt->cl_path.dentry)) {
		rpc_remove_client_dir(clnt->cl_path.dentry);
		rpc_put_mount();
	}
out_no_path:
	kfree(clnt->cl_principal);
out_no_principal:
	rpc_free_iostats(clnt->cl_metrics);
out_no_stats:
	if (clnt->cl_server != clnt->cl_inline_name)
		kfree(clnt->cl_server);
	kfree(clnt);
out_err:
	xprt_put(xprt);
out_no_xprt:
	rpciod_down();
out_no_rpciod:
	return ERR_PTR(err);
}