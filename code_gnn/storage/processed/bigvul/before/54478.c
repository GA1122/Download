int migrate_misplaced_transhuge_page(struct mm_struct *mm,
				struct vm_area_struct *vma,
				pmd_t *pmd, pmd_t entry,
				unsigned long address,
				struct page *page, int node)
{
	spinlock_t *ptl;
	pg_data_t *pgdat = NODE_DATA(node);
	int isolated = 0;
	struct page *new_page = NULL;
	int page_lru = page_is_file_cache(page);
	unsigned long mmun_start = address & HPAGE_PMD_MASK;
	unsigned long mmun_end = mmun_start + HPAGE_PMD_SIZE;
	pmd_t orig_entry;

	 
	if (numamigrate_update_ratelimit(pgdat, HPAGE_PMD_NR))
		goto out_dropref;

	new_page = alloc_pages_node(node,
		(GFP_TRANSHUGE | __GFP_THISNODE) & ~__GFP_WAIT,
		HPAGE_PMD_ORDER);
	if (!new_page)
		goto out_fail;

	isolated = numamigrate_isolate_page(pgdat, page);
	if (!isolated) {
		put_page(new_page);
		goto out_fail;
	}

	if (mm_tlb_flush_pending(mm))
		flush_tlb_range(vma, mmun_start, mmun_end);

	 
	__set_page_locked(new_page);
	SetPageSwapBacked(new_page);

	 
	new_page->mapping = page->mapping;
	new_page->index = page->index;
	migrate_page_copy(new_page, page);
	WARN_ON(PageLRU(new_page));

	 
	mmu_notifier_invalidate_range_start(mm, mmun_start, mmun_end);
	ptl = pmd_lock(mm, pmd);
	if (unlikely(!pmd_same(*pmd, entry) || page_count(page) != 2)) {
fail_putback:
		spin_unlock(ptl);
		mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);

		 
		if (TestClearPageActive(new_page))
			SetPageActive(page);
		if (TestClearPageUnevictable(new_page))
			SetPageUnevictable(page);

		unlock_page(new_page);
		put_page(new_page);		 

		 
		get_page(page);
		putback_lru_page(page);
		mod_zone_page_state(page_zone(page),
			 NR_ISOLATED_ANON + page_lru, -HPAGE_PMD_NR);

		goto out_unlock;
	}

	orig_entry = *pmd;
	entry = mk_pmd(new_page, vma->vm_page_prot);
	entry = pmd_mkhuge(entry);
	entry = maybe_pmd_mkwrite(pmd_mkdirty(entry), vma);

	 
	flush_cache_range(vma, mmun_start, mmun_end);
	page_add_anon_rmap(new_page, vma, mmun_start);
	pmdp_huge_clear_flush_notify(vma, mmun_start, pmd);
	set_pmd_at(mm, mmun_start, pmd, entry);
	flush_tlb_range(vma, mmun_start, mmun_end);
	update_mmu_cache_pmd(vma, address, &entry);

	if (page_count(page) != 2) {
		set_pmd_at(mm, mmun_start, pmd, orig_entry);
		flush_tlb_range(vma, mmun_start, mmun_end);
		mmu_notifier_invalidate_range(mm, mmun_start, mmun_end);
		update_mmu_cache_pmd(vma, address, &entry);
		page_remove_rmap(new_page);
		goto fail_putback;
	}

	mlock_migrate_page(new_page, page);
	set_page_memcg(new_page, page_memcg(page));
	set_page_memcg(page, NULL);
	page_remove_rmap(page);

	spin_unlock(ptl);
	mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);

	 
	get_page(new_page);
	putback_lru_page(new_page);

	unlock_page(new_page);
	unlock_page(page);
	put_page(page);			 
	put_page(page);			 

	count_vm_events(PGMIGRATE_SUCCESS, HPAGE_PMD_NR);
	count_vm_numa_events(NUMA_PAGE_MIGRATE, HPAGE_PMD_NR);

	mod_zone_page_state(page_zone(page),
			NR_ISOLATED_ANON + page_lru,
			-HPAGE_PMD_NR);
	return isolated;

out_fail:
	count_vm_events(PGMIGRATE_FAIL, HPAGE_PMD_NR);
out_dropref:
	ptl = pmd_lock(mm, pmd);
	if (pmd_same(*pmd, entry)) {
		entry = pmd_modify(entry, vma->vm_page_prot);
		set_pmd_at(mm, mmun_start, pmd, entry);
		update_mmu_cache_pmd(vma, address, &entry);
	}
	spin_unlock(ptl);

out_unlock:
	unlock_page(page);
	put_page(page);
	return 0;
}
