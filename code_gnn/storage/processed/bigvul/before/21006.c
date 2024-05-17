static void __mem_cgroup_commit_charge(struct mem_cgroup *memcg,
				       struct page *page,
				       unsigned int nr_pages,
				       struct page_cgroup *pc,
				       enum charge_type ctype,
				       bool lrucare)
{
	struct zone *uninitialized_var(zone);
	bool was_on_lru = false;

	lock_page_cgroup(pc);
	if (unlikely(PageCgroupUsed(pc))) {
		unlock_page_cgroup(pc);
		__mem_cgroup_cancel_charge(memcg, nr_pages);
		return;
	}
	 

	 
	if (lrucare) {
		zone = page_zone(page);
		spin_lock_irq(&zone->lru_lock);
		if (PageLRU(page)) {
			ClearPageLRU(page);
			del_page_from_lru_list(zone, page, page_lru(page));
			was_on_lru = true;
		}
	}

	pc->mem_cgroup = memcg;
	 
	smp_wmb();
	switch (ctype) {
	case MEM_CGROUP_CHARGE_TYPE_CACHE:
	case MEM_CGROUP_CHARGE_TYPE_SHMEM:
		SetPageCgroupCache(pc);
		SetPageCgroupUsed(pc);
		break;
	case MEM_CGROUP_CHARGE_TYPE_MAPPED:
		ClearPageCgroupCache(pc);
		SetPageCgroupUsed(pc);
		break;
	default:
		break;
	}

	if (lrucare) {
		if (was_on_lru) {
			VM_BUG_ON(PageLRU(page));
			SetPageLRU(page);
			add_page_to_lru_list(zone, page, page_lru(page));
		}
		spin_unlock_irq(&zone->lru_lock);
	}

	mem_cgroup_charge_statistics(memcg, PageCgroupCache(pc), nr_pages);
	unlock_page_cgroup(pc);

	 
	memcg_check_events(memcg, page);
}
