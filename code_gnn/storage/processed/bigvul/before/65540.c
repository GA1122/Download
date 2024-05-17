static int nfs4_setlease(struct nfs4_delegation *dp)
{
	struct nfs4_file *fp = dp->dl_stid.sc_file;
	struct file_lock *fl;
	struct file *filp;
	int status = 0;

	fl = nfs4_alloc_init_lease(fp, NFS4_OPEN_DELEGATE_READ);
	if (!fl)
		return -ENOMEM;
	filp = find_readable_file(fp);
	if (!filp) {
		 
		WARN_ON_ONCE(1);
		locks_free_lock(fl);
		return -EBADF;
	}
	fl->fl_file = filp;
	status = vfs_setlease(filp, fl->fl_type, &fl, NULL);
	if (fl)
		locks_free_lock(fl);
	if (status)
		goto out_fput;
	spin_lock(&state_lock);
	spin_lock(&fp->fi_lock);
	 
	status = -EAGAIN;
	if (fp->fi_had_conflict)
		goto out_unlock;
	 
	if (fp->fi_deleg_file) {
		status = hash_delegation_locked(dp, fp);
		goto out_unlock;
	}
	fp->fi_deleg_file = filp;
	fp->fi_delegees = 0;
	status = hash_delegation_locked(dp, fp);
	spin_unlock(&fp->fi_lock);
	spin_unlock(&state_lock);
	if (status) {
		 
		WARN_ON_ONCE(1);
		goto out_fput;
	}
	return 0;
out_unlock:
	spin_unlock(&fp->fi_lock);
	spin_unlock(&state_lock);
out_fput:
	fput(filp);
	return status;
}