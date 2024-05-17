int hugetlb_fault(struct mm_struct *mm, struct vm_area_struct *vma,
			unsigned long address, unsigned int flags)
{
	pte_t *ptep;
	pte_t entry;
	int ret;
	struct page *page = NULL;
	struct page *pagecache_page = NULL;
	static DEFINE_MUTEX(hugetlb_instantiation_mutex);
	struct hstate *h = hstate_vma(vma);

	address &= huge_page_mask(h);

	ptep = huge_pte_offset(mm, address);
	if (ptep) {
		entry = huge_ptep_get(ptep);
		if (unlikely(is_hugetlb_entry_migration(entry))) {
			migration_entry_wait(mm, (pmd_t *)ptep, address);
			return 0;
		} else if (unlikely(is_hugetlb_entry_hwpoisoned(entry)))
			return VM_FAULT_HWPOISON_LARGE |
			       VM_FAULT_SET_HINDEX(h - hstates);
	}

	ptep = huge_pte_alloc(mm, address, huge_page_size(h));
	if (!ptep)
		return VM_FAULT_OOM;

	 
	mutex_lock(&hugetlb_instantiation_mutex);
	entry = huge_ptep_get(ptep);
	if (huge_pte_none(entry)) {
		ret = hugetlb_no_page(mm, vma, address, ptep, flags);
		goto out_mutex;
	}

	ret = 0;

	 
	if ((flags & FAULT_FLAG_WRITE) && !pte_write(entry)) {
		if (vma_needs_reservation(h, vma, address) < 0) {
			ret = VM_FAULT_OOM;
			goto out_mutex;
		}

		if (!(vma->vm_flags & VM_MAYSHARE))
			pagecache_page = hugetlbfs_pagecache_page(h,
								vma, address);
	}

	 
	page = pte_page(entry);
	get_page(page);
	if (page != pagecache_page)
		lock_page(page);

	spin_lock(&mm->page_table_lock);
	 
	if (unlikely(!pte_same(entry, huge_ptep_get(ptep))))
		goto out_page_table_lock;


	if (flags & FAULT_FLAG_WRITE) {
		if (!pte_write(entry)) {
			ret = hugetlb_cow(mm, vma, address, ptep, entry,
							pagecache_page);
			goto out_page_table_lock;
		}
		entry = pte_mkdirty(entry);
	}
	entry = pte_mkyoung(entry);
	if (huge_ptep_set_access_flags(vma, address, ptep, entry,
						flags & FAULT_FLAG_WRITE))
		update_mmu_cache(vma, address, ptep);

out_page_table_lock:
	spin_unlock(&mm->page_table_lock);

	if (pagecache_page) {
		unlock_page(pagecache_page);
		put_page(pagecache_page);
	}
	if (page != pagecache_page)
		unlock_page(page);
	put_page(page);

out_mutex:
	mutex_unlock(&hugetlb_instantiation_mutex);

	return ret;
}
