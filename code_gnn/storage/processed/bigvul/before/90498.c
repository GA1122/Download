static pagemap_entry_t pte_to_pagemap_entry(struct pagemapread *pm,
		struct vm_area_struct *vma, unsigned long addr, pte_t pte)
{
	u64 frame = 0, flags = 0;
	struct page *page = NULL;

	if (pte_present(pte)) {
		if (pm->show_pfn)
			frame = pte_pfn(pte);
		flags |= PM_PRESENT;
		page = _vm_normal_page(vma, addr, pte, true);
		if (pte_soft_dirty(pte))
			flags |= PM_SOFT_DIRTY;
	} else if (is_swap_pte(pte)) {
		swp_entry_t entry;
		if (pte_swp_soft_dirty(pte))
			flags |= PM_SOFT_DIRTY;
		entry = pte_to_swp_entry(pte);
		if (pm->show_pfn)
			frame = swp_type(entry) |
				(swp_offset(entry) << MAX_SWAPFILES_SHIFT);
		flags |= PM_SWAP;
		if (is_migration_entry(entry))
			page = migration_entry_to_page(entry);

		if (is_device_private_entry(entry))
			page = device_private_entry_to_page(entry);
	}

	if (page && !PageAnon(page))
		flags |= PM_FILE;
	if (page && page_mapcount(page) == 1)
		flags |= PM_MMAP_EXCLUSIVE;
	if (vma->vm_flags & VM_SOFTDIRTY)
		flags |= PM_SOFT_DIRTY;

	return make_pme(frame, flags);
}