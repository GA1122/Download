unhash_delegation_locked(struct nfs4_delegation *dp)
{
	struct nfs4_file *fp = dp->dl_stid.sc_file;

	lockdep_assert_held(&state_lock);

	if (list_empty(&dp->dl_perfile))
		return false;

	dp->dl_stid.sc_type = NFS4_CLOSED_DELEG_STID;
	 
	++dp->dl_time;
	spin_lock(&fp->fi_lock);
	list_del_init(&dp->dl_perclnt);
	list_del_init(&dp->dl_recall_lru);
	list_del_init(&dp->dl_perfile);
	spin_unlock(&fp->fi_lock);
	return true;
}