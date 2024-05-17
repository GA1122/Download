static void __d_rehash(struct dentry * entry, struct hlist_bl_head *b)
{
	BUG_ON(!d_unhashed(entry));
	hlist_bl_lock(b);
	entry->d_flags |= DCACHE_RCUACCESS;
	hlist_bl_add_head_rcu(&entry->d_hash, b);
	hlist_bl_unlock(b);
}