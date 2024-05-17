lookup_or_create_lock_state(struct nfsd4_compound_state *cstate,
			    struct nfs4_ol_stateid *ost,
			    struct nfsd4_lock *lock,
			    struct nfs4_ol_stateid **plst, bool *new)
{
	__be32 status;
	struct nfs4_file *fi = ost->st_stid.sc_file;
	struct nfs4_openowner *oo = openowner(ost->st_stateowner);
	struct nfs4_client *cl = oo->oo_owner.so_client;
	struct inode *inode = d_inode(cstate->current_fh.fh_dentry);
	struct nfs4_lockowner *lo;
	struct nfs4_ol_stateid *lst;
	unsigned int strhashval;
	bool hashed;

	lo = find_lockowner_str(cl, &lock->lk_new_owner);
	if (!lo) {
		strhashval = ownerstr_hashval(&lock->lk_new_owner);
		lo = alloc_init_lock_stateowner(strhashval, cl, ost, lock);
		if (lo == NULL)
			return nfserr_jukebox;
	} else {
		 
		status = nfserr_bad_seqid;
		if (!cstate->minorversion &&
		    lock->lk_new_lock_seqid != lo->lo_owner.so_seqid)
			goto out;
	}

retry:
	lst = find_or_create_lock_stateid(lo, fi, inode, ost, new);
	if (lst == NULL) {
		status = nfserr_jukebox;
		goto out;
	}

	mutex_lock(&lst->st_mutex);

	 
	spin_lock(&cl->cl_lock);
	hashed = !list_empty(&lst->st_perfile);
	spin_unlock(&cl->cl_lock);

	if (!hashed) {
		mutex_unlock(&lst->st_mutex);
		nfs4_put_stid(&lst->st_stid);
		goto retry;
	}
	status = nfs_ok;
	*plst = lst;
out:
	nfs4_put_stateowner(&lo->lo_owner);
	return status;
}