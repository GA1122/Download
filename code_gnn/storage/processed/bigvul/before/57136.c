nfs4_init_nonuniform_client_string(struct nfs_client *clp)
{
	int result;
	size_t len;
	char *str;
	bool retried = false;

	if (clp->cl_owner_id != NULL)
		return 0;
retry:
	rcu_read_lock();
	len = 10 + strlen(clp->cl_ipaddr) + 1 +
		strlen(rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_ADDR)) +
		1 +
		strlen(rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_PROTO)) +
		1;
	rcu_read_unlock();

	if (len > NFS4_OPAQUE_LIMIT + 1)
		return -EINVAL;

	 
	str = kmalloc(len, GFP_KERNEL);
	if (!str)
		return -ENOMEM;

	rcu_read_lock();
	result = scnprintf(str, len, "Linux NFSv4.0 %s/%s %s",
			clp->cl_ipaddr,
			rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_ADDR),
			rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_PROTO));
	rcu_read_unlock();

	 
	if (result >= len) {
		kfree(str);
		if (retried)
			return -EINVAL;
		retried = true;
		goto retry;
	}
	clp->cl_owner_id = str;
	return 0;
}