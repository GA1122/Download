int nfs4_proc_secinfo(struct inode *dir, const struct qstr *name,
		      struct nfs4_secinfo_flavors *flavors)
{
	struct nfs4_exception exception = { };
	int err;
	do {
		err = -NFS4ERR_WRONGSEC;

		 
		if (_nfs4_is_integrity_protected(NFS_SERVER(dir)->nfs_client))
			err = _nfs4_proc_secinfo(dir, name, flavors, true);

		 
		if (err == -NFS4ERR_WRONGSEC)
			err = _nfs4_proc_secinfo(dir, name, flavors, false);

		trace_nfs4_secinfo(dir, name, err);
		err = nfs4_handle_exception(NFS_SERVER(dir), err,
				&exception);
	} while (exception.retry);
	return err;
}
