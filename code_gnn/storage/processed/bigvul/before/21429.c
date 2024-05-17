__mem_cgroup_commit_charge_lrucare(struct page *page, struct mem_cgroup *memcg,
					enum charge_type ctype)
{
	struct page_cgroup *pc = lookup_page_cgroup(page);
	struct zone *zone = page_zone(page);
	unsigned long flags;
	bool removed = false;

	 
	spin_lock_irqsave(&zone->lru_lock, flags);
	if (PageLRU(page)) {
		del_page_from_lru_list(zone, page, page_lru(page));
		ClearPageLRU(page);
		removed = true;
	}
	__mem_cgroup_commit_charge(memcg, page, 1, pc, ctype);
	if (removed) {
		add_page_to_lru_list(zone, page, page_lru(page));
		SetPageLRU(page);
	}
	spin_unlock_irqrestore(&zone->lru_lock, flags);
	return;
}
