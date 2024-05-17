static int shmem_unuse_inode(struct shmem_inode_info *info,
			     swp_entry_t swap, struct page **pagep)
{
	struct address_space *mapping = info->vfs_inode.i_mapping;
	void *radswap;
	pgoff_t index;
	gfp_t gfp;
	int error = 0;

	radswap = swp_to_radix_entry(swap);
	index = radix_tree_locate_item(&mapping->page_tree, radswap);
	if (index == -1)
		return 0;

	 
	if (shmem_swaplist.next != &info->swaplist)
		list_move_tail(&shmem_swaplist, &info->swaplist);

	gfp = mapping_gfp_mask(mapping);
	if (shmem_should_replace_page(*pagep, gfp)) {
		mutex_unlock(&shmem_swaplist_mutex);
		error = shmem_replace_page(pagep, gfp, info, index);
		mutex_lock(&shmem_swaplist_mutex);
		 
		if (!page_swapcount(*pagep))
			error = -ENOENT;
	}

	 
	if (!error)
		error = shmem_add_to_page_cache(*pagep, mapping, index,
						GFP_NOWAIT, radswap);
	if (error != -ENOMEM) {
		 
		delete_from_swap_cache(*pagep);
		set_page_dirty(*pagep);
		if (!error) {
			spin_lock(&info->lock);
			info->swapped--;
			spin_unlock(&info->lock);
			swap_free(swap);
		}
		error = 1;	 
	}
	return error;
}
