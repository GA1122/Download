static enum lru_status dentry_lru_isolate_shrink(struct list_head *item,
		struct list_lru_one *lru, spinlock_t *lru_lock, void *arg)
{
	struct list_head *freeable = arg;
	struct dentry	*dentry = container_of(item, struct dentry, d_lru);

	 
	if (!spin_trylock(&dentry->d_lock))
		return LRU_SKIP;

	d_lru_shrink_move(lru, dentry, freeable);
	spin_unlock(&dentry->d_lock);

	return LRU_REMOVED;
}