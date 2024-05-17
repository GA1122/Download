static void __split_huge_page_refcount(struct page *page)
{
	int i;
	unsigned long head_index = page->index;
	struct zone *zone = page_zone(page);
	int zonestat;

	 
	spin_lock_irq(&zone->lru_lock);
	compound_lock(page);

	for (i = 1; i < HPAGE_PMD_NR; i++) {
		struct page *page_tail = page + i;

		 
		atomic_sub(atomic_read(&page_tail->_count), &page->_count);
		BUG_ON(page_count(page) <= 0);
		atomic_add(page_mapcount(page) + 1, &page_tail->_count);
		BUG_ON(atomic_read(&page_tail->_count) <= 0);

		 
		smp_mb();

		 
		page_tail->flags &= ~PAGE_FLAGS_CHECK_AT_PREP | __PG_HWPOISON;
		page_tail->flags |= (page->flags &
				     ((1L << PG_referenced) |
				      (1L << PG_swapbacked) |
				      (1L << PG_mlocked) |
				      (1L << PG_uptodate)));
		page_tail->flags |= (1L << PG_dirty);

		 
		smp_wmb();

		 
		BUG_ON(page_mapcount(page_tail));
		page_tail->_mapcount = page->_mapcount;

		BUG_ON(page_tail->mapping);
		page_tail->mapping = page->mapping;

		page_tail->index = ++head_index;

		BUG_ON(!PageAnon(page_tail));
		BUG_ON(!PageUptodate(page_tail));
		BUG_ON(!PageDirty(page_tail));
		BUG_ON(!PageSwapBacked(page_tail));

		mem_cgroup_split_huge_fixup(page, page_tail);

		lru_add_page_tail(zone, page, page_tail);
	}

	__dec_zone_page_state(page, NR_ANON_TRANSPARENT_HUGEPAGES);
	__mod_zone_page_state(zone, NR_ANON_PAGES, HPAGE_PMD_NR);

	 
	if (PageLRU(page)) {
		zonestat = NR_LRU_BASE + page_lru(page);
		__mod_zone_page_state(zone, zonestat, -(HPAGE_PMD_NR-1));
	}

	ClearPageCompound(page);
	compound_unlock(page);
	spin_unlock_irq(&zone->lru_lock);

	for (i = 1; i < HPAGE_PMD_NR; i++) {
		struct page *page_tail = page + i;
		BUG_ON(page_count(page_tail) <= 0);
		 
		put_page(page_tail);
	}

	 
	BUG_ON(page_count(page) <= 0);
}
