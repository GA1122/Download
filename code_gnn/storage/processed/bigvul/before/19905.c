static int nfs4_find_root_sec(struct nfs_server *server, struct nfs_fh *fhandle,
			      struct nfs_fsinfo *info)
{
	int i, len, status = 0;
	rpc_authflavor_t flav_array[NFS_MAX_SECFLAVORS];

	len = gss_mech_list_pseudoflavors(&flav_array[0]);
	flav_array[len] = RPC_AUTH_NULL;
	len += 1;

	for (i = 0; i < len; i++) {
		status = nfs4_lookup_root_sec(server, fhandle, info, flav_array[i]);
		if (status == -NFS4ERR_WRONGSEC || status == -EACCES)
			continue;
		break;
	}
	 
	if (status == -EACCES)
		status = -EPERM;
	return status;
}