unsigned int munlock_vma_page(struct page *page)
{
	int nr_pages;
	struct zone *zone = page_zone(page);

	 
	BUG_ON(!PageLocked(page));

	VM_BUG_ON_PAGE(PageTail(page), page);

	 
	spin_lock_irq(zone_lru_lock(zone));

	if (!TestClearPageMlocked(page)) {
		 
		nr_pages = 1;
		goto unlock_out;
	}

	nr_pages = hpage_nr_pages(page);
	__mod_zone_page_state(zone, NR_MLOCK, -nr_pages);

	if (__munlock_isolate_lru_page(page, true)) {
		spin_unlock_irq(zone_lru_lock(zone));
		__munlock_isolated_page(page);
		goto out;
	}
	__munlock_isolation_failed(page);

unlock_out:
	spin_unlock_irq(zone_lru_lock(zone));

out:
	return nr_pages - 1;
}
