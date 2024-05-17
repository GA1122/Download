 int migrate_page_move_mapping(struct address_space *mapping,
 		struct page *newpage, struct page *page,
  		struct buffer_head *head, enum migrate_mode mode,
  		int extra_count)
  {
// 	struct zone *oldzone, *newzone;
// 	int dirty;
  	int expected_count = 1 + extra_count;
  	void **pslot;
  
 	if (!mapping) {
 		 
 		if (page_count(page) != expected_count)
 			return -EAGAIN;
 
 		 
 		set_page_memcg(newpage, page_memcg(page));
 		newpage->index = page->index;
 		newpage->mapping = page->mapping;
 		if (PageSwapBacked(page))
 			SetPageSwapBacked(newpage);
 
  		return MIGRATEPAGE_SUCCESS;
  	}
  
// 	oldzone = page_zone(page);
// 	newzone = page_zone(newpage);
// 
  	spin_lock_irq(&mapping->tree_lock);
  
  	pslot = radix_tree_lookup_slot(&mapping->page_tree,
  					page_index(page));
 
 	expected_count += 1 + page_has_private(page);
 	if (page_count(page) != expected_count ||
 		radix_tree_deref_slot_protected(pslot, &mapping->tree_lock) != page) {
 		spin_unlock_irq(&mapping->tree_lock);
 		return -EAGAIN;
 	}
 
 	if (!page_freeze_refs(page, expected_count)) {
 		spin_unlock_irq(&mapping->tree_lock);
 		return -EAGAIN;
 	}
 
 	 
 	if (mode == MIGRATE_ASYNC && head &&
 			!buffer_migrate_lock_buffers(head, mode)) {
 		page_unfreeze_refs(page, expected_count);
 		spin_unlock_irq(&mapping->tree_lock);
 		return -EAGAIN;
 	}
 
 	 
 	set_page_memcg(newpage, page_memcg(page));
 	newpage->index = page->index;
 	newpage->mapping = page->mapping;
 	if (PageSwapBacked(page))
 		SetPageSwapBacked(newpage);
 
 	get_page(newpage);	 
 	if (PageSwapCache(page)) {
 		SetPageSwapCache(newpage);
  		set_page_private(newpage, page_private(page));
  	}
  
// 	 
// 	dirty = PageDirty(page);
// 	if (dirty) {
// 		ClearPageDirty(page);
// 		SetPageDirty(newpage);
// 	}
// 
  	radix_tree_replace_slot(pslot, newpage);
  
  	 
  	page_unfreeze_refs(page, expected_count - 1);
  
// 	spin_unlock(&mapping->tree_lock);
// 	 
// 
  	 
	__dec_zone_page_state(page, NR_FILE_PAGES);
	__inc_zone_page_state(newpage, NR_FILE_PAGES);
	if (!PageSwapCache(page) && PageSwapBacked(page)) {
		__dec_zone_page_state(page, NR_SHMEM);
		__inc_zone_page_state(newpage, NR_SHMEM);
// 	if (newzone != oldzone) {
// 		__dec_zone_state(oldzone, NR_FILE_PAGES);
// 		__inc_zone_state(newzone, NR_FILE_PAGES);
// 		if (PageSwapBacked(page) && !PageSwapCache(page)) {
// 			__dec_zone_state(oldzone, NR_SHMEM);
// 			__inc_zone_state(newzone, NR_SHMEM);
// 		}
// 		if (dirty && mapping_cap_account_dirty(mapping)) {
// 			__dec_zone_state(oldzone, NR_FILE_DIRTY);
// 			__inc_zone_state(newzone, NR_FILE_DIRTY);
// 		}
  	}
	spin_unlock_irq(&mapping->tree_lock);
// 	local_irq_enable();
  
  	return MIGRATEPAGE_SUCCESS;
  }