static inline bool fast_dput(struct dentry *dentry)
{
	int ret;
	unsigned int d_flags;

	 
	if (unlikely(dentry->d_flags & DCACHE_OP_DELETE))
		return lockref_put_or_lock(&dentry->d_lockref);

	 
	ret = lockref_put_return(&dentry->d_lockref);

	 
	if (unlikely(ret < 0)) {
		spin_lock(&dentry->d_lock);
		if (dentry->d_lockref.count > 1) {
			dentry->d_lockref.count--;
			spin_unlock(&dentry->d_lock);
			return 1;
		}
		return 0;
	}

	 
	if (ret)
		return 1;

	 
	smp_rmb();
	d_flags = ACCESS_ONCE(dentry->d_flags);
	d_flags &= DCACHE_REFERENCED | DCACHE_LRU_LIST | DCACHE_DISCONNECTED;

	 
	if (d_flags == (DCACHE_REFERENCED | DCACHE_LRU_LIST) && !d_unhashed(dentry))
		return 1;

	 
	spin_lock(&dentry->d_lock);

	 
	if (dentry->d_lockref.count) {
		spin_unlock(&dentry->d_lock);
		return 1;
	}

	 
	dentry->d_lockref.count = 1;
	return 0;
}
