nfsd_break_deleg_cb(struct file_lock *fl)
{
	bool ret = false;
	struct nfs4_file *fp = (struct nfs4_file *)fl->fl_owner;
	struct nfs4_delegation *dp;

	if (!fp) {
		WARN(1, "(%p)->fl_owner NULL\n", fl);
		return ret;
	}
	if (fp->fi_had_conflict) {
		WARN(1, "duplicate break on %p\n", fp);
		return ret;
	}
	 
	fl->fl_break_time = 0;

	spin_lock(&fp->fi_lock);
	fp->fi_had_conflict = true;
	 
	if (list_empty(&fp->fi_delegations))
		ret = true;
	else
		list_for_each_entry(dp, &fp->fi_delegations, dl_perfile)
			nfsd_break_one_deleg(dp);
	spin_unlock(&fp->fi_lock);
	return ret;
}
