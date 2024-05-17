static int nfs4_find_root_sec(struct nfs_server *server, struct nfs_fh *fhandle,
			      struct nfs_fsinfo *info)
{
	int i, len, status = 0;
	rpc_authflavor_t flav_array[NFS_MAX_SECFLAVORS];

	len = rpcauth_list_flavors(flav_array, ARRAY_SIZE(flav_array));
	BUG_ON(len < 0);

	for (i = 0; i < len; i++) {
		 
		if (flav_array[i] == RPC_AUTH_UNIX)
			continue;

		status = nfs4_lookup_root_sec(server, fhandle, info, flav_array[i]);
		if (status == -NFS4ERR_WRONGSEC || status == -EACCES)
			continue;
		break;
	}
	 
	if (status == -EACCES)
		status = -EPERM;
	return status;
}
