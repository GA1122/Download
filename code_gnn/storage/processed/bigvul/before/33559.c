int shmem_unuse(swp_entry_t swap, struct page *page)
{
	struct list_head *this, *next;
	struct shmem_inode_info *info;
	int found = 0;
	int error = 0;

	 
	if (unlikely(!PageSwapCache(page) || page_private(page) != swap.val))
		goto out;

	 
	error = mem_cgroup_cache_charge(page, current->mm, GFP_KERNEL);
	if (error)
		goto out;
	 

	mutex_lock(&shmem_swaplist_mutex);
	list_for_each_safe(this, next, &shmem_swaplist) {
		info = list_entry(this, struct shmem_inode_info, swaplist);
		if (info->swapped)
			found = shmem_unuse_inode(info, swap, &page);
		else
			list_del_init(&info->swaplist);
		cond_resched();
		if (found)
			break;
	}
	mutex_unlock(&shmem_swaplist_mutex);

	if (found < 0)
		error = found;
out:
	unlock_page(page);
	page_cache_release(page);
	return error;
}
