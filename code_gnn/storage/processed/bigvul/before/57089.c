nfs41_proc_secinfo_no_name(struct nfs_server *server, struct nfs_fh *fhandle,
			   struct nfs_fsinfo *info, struct nfs4_secinfo_flavors *flavors)
{
	struct nfs4_exception exception = { };
	int err;
	do {
		 
		err = -NFS4ERR_WRONGSEC;

		 
		if (_nfs4_is_integrity_protected(server->nfs_client))
			err = _nfs41_proc_secinfo_no_name(server, fhandle, info,
							  flavors, true);

		 
		if (err == -NFS4ERR_WRONGSEC)
			err = _nfs41_proc_secinfo_no_name(server, fhandle, info,
							  flavors, false);

		switch (err) {
		case 0:
		case -NFS4ERR_WRONGSEC:
		case -ENOTSUPP:
			goto out;
		default:
			err = nfs4_handle_exception(server, err, &exception);
		}
	} while (exception.retry);
out:
	return err;
}