static int nfs4_find_root_sec(struct nfs_server *server, struct nfs_fh *fhandle,
			      struct nfs_fsinfo *info)
{
	 
	static const rpc_authflavor_t flav_array[] = {
		RPC_AUTH_GSS_KRB5P,
		RPC_AUTH_GSS_KRB5I,
		RPC_AUTH_GSS_KRB5,
		RPC_AUTH_UNIX,			 
		RPC_AUTH_NULL,
	};
	int status = -EPERM;
	size_t i;

	if (server->auth_info.flavor_len > 0) {
		 
		for (i = 0; i < server->auth_info.flavor_len; i++) {
			status = nfs4_lookup_root_sec(server, fhandle, info,
						server->auth_info.flavors[i]);
			if (status == -NFS4ERR_WRONGSEC || status == -EACCES)
				continue;
			break;
		}
	} else {
		 
		for (i = 0; i < ARRAY_SIZE(flav_array); i++) {
			status = nfs4_lookup_root_sec(server, fhandle, info,
						      flav_array[i]);
			if (status == -NFS4ERR_WRONGSEC || status == -EACCES)
				continue;
			break;
		}
	}

	 
	if (status == -EACCES)
		status = -EPERM;
	return status;
}
