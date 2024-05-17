static inline void dentry_rcuwalk_invalidate(struct dentry *dentry)
{
	lockdep_assert_held(&dentry->d_lock);
	 
	write_seqcount_invalidate(&dentry->d_seq);
}
