static int lookup_fast(struct nameidata *nd,
		       struct path *path, struct inode **inode,
		       unsigned *seqp)
{
	struct vfsmount *mnt = nd->path.mnt;
	struct dentry *dentry, *parent = nd->path.dentry;
	int need_reval = 1;
	int status = 1;
	int err;

	 
	if (nd->flags & LOOKUP_RCU) {
		unsigned seq;
		bool negative;
		dentry = __d_lookup_rcu(parent, &nd->last, &seq);
		if (!dentry)
			goto unlazy;

		 
		*inode = d_backing_inode(dentry);
		negative = d_is_negative(dentry);
		if (read_seqcount_retry(&dentry->d_seq, seq))
			return -ECHILD;
		if (negative)
			return -ENOENT;

		 
		if (__read_seqcount_retry(&parent->d_seq, nd->seq))
			return -ECHILD;

		*seqp = seq;
		if (unlikely(dentry->d_flags & DCACHE_OP_REVALIDATE)) {
			status = d_revalidate(dentry, nd->flags);
			if (unlikely(status <= 0)) {
				if (status != -ECHILD)
					need_reval = 0;
				goto unlazy;
			}
		}
		path->mnt = mnt;
		path->dentry = dentry;
		if (likely(__follow_mount_rcu(nd, path, inode, seqp)))
			return 0;
unlazy:
		if (unlazy_walk(nd, dentry, seq))
			return -ECHILD;
	} else {
		dentry = __d_lookup(parent, &nd->last);
	}

	if (unlikely(!dentry))
		goto need_lookup;

	if (unlikely(dentry->d_flags & DCACHE_OP_REVALIDATE) && need_reval)
		status = d_revalidate(dentry, nd->flags);
	if (unlikely(status <= 0)) {
		if (status < 0) {
			dput(dentry);
			return status;
		}
		d_invalidate(dentry);
		dput(dentry);
		goto need_lookup;
	}

	if (unlikely(d_is_negative(dentry))) {
		dput(dentry);
		return -ENOENT;
	}
	path->mnt = mnt;
	path->dentry = dentry;
	err = follow_managed(path, nd);
	if (likely(!err))
		*inode = d_backing_inode(path->dentry);
	return err;

need_lookup:
	return 1;
}
