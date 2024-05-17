nfs4_init_uniform_client_string(struct nfs_client *clp)
{
	int result;
	size_t len;
	char *str;

	if (clp->cl_owner_id != NULL)
		return 0;

	if (nfs4_client_id_uniquifier[0] != '\0')
		return nfs4_init_uniquifier_client_string(clp);

	len = 10 + 10 + 1 + 10 + 1 +
		strlen(clp->cl_rpcclient->cl_nodename) + 1;

	if (len > NFS4_OPAQUE_LIMIT + 1)
		return -EINVAL;

	 
	str = kmalloc(len, GFP_KERNEL);
	if (!str)
		return -ENOMEM;

	result = scnprintf(str, len, "Linux NFSv%u.%u %s",
			clp->rpc_ops->version, clp->cl_minorversion,
			clp->cl_rpcclient->cl_nodename);
	if (result >= len) {
		kfree(str);
		return -EINVAL;
	}
	clp->cl_owner_id = str;
	return 0;
}