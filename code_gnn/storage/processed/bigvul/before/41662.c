bool btrfs_page_exists_in_range(struct inode *inode, loff_t start, loff_t end)
{
	struct radix_tree_root *root = &inode->i_mapping->page_tree;
	int found = false;
	void **pagep = NULL;
	struct page *page = NULL;
	int start_idx;
	int end_idx;

	start_idx = start >> PAGE_CACHE_SHIFT;

	 
	end_idx = end >> PAGE_CACHE_SHIFT;

	rcu_read_lock();

	 
	while (page == NULL &&
	       radix_tree_gang_lookup_slot(root, &pagep, NULL, start_idx, 1)) {
		page = radix_tree_deref_slot(pagep);
		if (unlikely(!page))
			break;

		if (radix_tree_exception(page)) {
			if (radix_tree_deref_retry(page)) {
				page = NULL;
				continue;
			}
			 
			page = NULL;
			break;  
		}

		if (!page_cache_get_speculative(page)) {
			page = NULL;
			continue;
		}

		 
		if (unlikely(page != *pagep)) {
			page_cache_release(page);
			page = NULL;
		}
	}

	if (page) {
		if (page->index <= end_idx)
			found = true;
		page_cache_release(page);
	}

	rcu_read_unlock();
	return found;
}
