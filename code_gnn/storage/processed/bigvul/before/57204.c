static int nfs4_proc_mknod(struct inode *dir, struct dentry *dentry,
		struct iattr *sattr, dev_t rdev)
{
	struct nfs4_exception exception = { };
	struct nfs4_label l, *label = NULL;
	int err;

	label = nfs4_label_init_security(dir, dentry, sattr, &l);

	sattr->ia_mode &= ~current_umask();
	do {
		err = _nfs4_proc_mknod(dir, dentry, sattr, label, rdev);
		trace_nfs4_mknod(dir, &dentry->d_name, err);
		err = nfs4_handle_exception(NFS_SERVER(dir), err,
				&exception);
	} while (exception.retry);

	nfs4_label_release_security(label);

	return err;
}