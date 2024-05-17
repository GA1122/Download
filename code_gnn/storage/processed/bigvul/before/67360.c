static inline struct dentry *lock_parent(struct dentry *dentry)
{
	struct dentry *parent = dentry->d_parent;
	if (IS_ROOT(dentry))
		return NULL;
	if (unlikely(dentry->d_lockref.count < 0))
		return NULL;
	if (likely(spin_trylock(&parent->d_lock)))
		return parent;
	rcu_read_lock();
	spin_unlock(&dentry->d_lock);
again:
	parent = ACCESS_ONCE(dentry->d_parent);
	spin_lock(&parent->d_lock);
	 
	if (unlikely(parent != dentry->d_parent)) {
		spin_unlock(&parent->d_lock);
		goto again;
	}
	rcu_read_unlock();
	if (parent != dentry)
		spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
	else
		parent = NULL;
	return parent;
}