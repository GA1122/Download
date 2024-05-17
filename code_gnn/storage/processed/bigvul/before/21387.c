static void free_swap_count_continuations(struct swap_info_struct *si)
{
	pgoff_t offset;

	for (offset = 0; offset < si->max; offset += PAGE_SIZE) {
		struct page *head;
		head = vmalloc_to_page(si->swap_map + offset);
		if (page_private(head)) {
			struct list_head *this, *next;
			list_for_each_safe(this, next, &head->lru) {
				struct page *page;
				page = list_entry(this, struct page, lru);
				list_del(this);
				__free_page(page);
			}
		}
	}
}
