struct page *find_lock_page(struct address_space *mapping,
				pgoff_t offset)
{
	struct page *page;

repeat:
	read_lock_irq(&mapping->tree_lock);
	page = radix_tree_lookup(&mapping->page_tree, offset);
	if (page) {
		page_cache_get(page);
		if (TestSetPageLocked(page)) {
			read_unlock_irq(&mapping->tree_lock);
			__lock_page(page);

			 
			if (unlikely(page->mapping != mapping)) {
				unlock_page(page);
				page_cache_release(page);
				goto repeat;
			}
			VM_BUG_ON(page->index != offset);
			goto out;
		}
	}
	read_unlock_irq(&mapping->tree_lock);
out:
	return page;
}