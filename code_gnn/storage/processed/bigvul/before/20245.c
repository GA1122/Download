static int hugetlb_cow(struct mm_struct *mm, struct vm_area_struct *vma,
			unsigned long address, pte_t *ptep, pte_t pte,
			struct page *pagecache_page)
{
	struct hstate *h = hstate_vma(vma);
	struct page *old_page, *new_page;
	int avoidcopy;
	int outside_reserve = 0;

	old_page = pte_page(pte);

retry_avoidcopy:
	 
	avoidcopy = (page_mapcount(old_page) == 1);
	if (avoidcopy) {
		if (PageAnon(old_page))
			page_move_anon_rmap(old_page, vma, address);
		set_huge_ptep_writable(vma, address, ptep);
		return 0;
	}

	 
	if (!(vma->vm_flags & VM_MAYSHARE) &&
			is_vma_resv_set(vma, HPAGE_RESV_OWNER) &&
			old_page != pagecache_page)
		outside_reserve = 1;

	page_cache_get(old_page);

	 
	spin_unlock(&mm->page_table_lock);
	new_page = alloc_huge_page(vma, address, outside_reserve);

	if (IS_ERR(new_page)) {
		page_cache_release(old_page);

		 
		if (outside_reserve) {
			BUG_ON(huge_pte_none(pte));
			if (unmap_ref_private(mm, vma, old_page, address)) {
				BUG_ON(page_count(old_page) != 1);
				BUG_ON(huge_pte_none(pte));
				spin_lock(&mm->page_table_lock);
				ptep = huge_pte_offset(mm, address & huge_page_mask(h));
				if (likely(pte_same(huge_ptep_get(ptep), pte)))
					goto retry_avoidcopy;
				 
				return 0;
			}
			WARN_ON_ONCE(1);
		}

		 
		spin_lock(&mm->page_table_lock);
		return -PTR_ERR(new_page);
	}

	 
	if (unlikely(anon_vma_prepare(vma))) {
		page_cache_release(new_page);
		page_cache_release(old_page);
		 
		spin_lock(&mm->page_table_lock);
		return VM_FAULT_OOM;
	}

	copy_user_huge_page(new_page, old_page, address, vma,
			    pages_per_huge_page(h));
	__SetPageUptodate(new_page);

	 
	spin_lock(&mm->page_table_lock);
	ptep = huge_pte_offset(mm, address & huge_page_mask(h));
	if (likely(pte_same(huge_ptep_get(ptep), pte))) {
		 
		mmu_notifier_invalidate_range_start(mm,
			address & huge_page_mask(h),
			(address & huge_page_mask(h)) + huge_page_size(h));
		huge_ptep_clear_flush(vma, address, ptep);
		set_huge_pte_at(mm, address, ptep,
				make_huge_pte(vma, new_page, 1));
		page_remove_rmap(old_page);
		hugepage_add_new_anon_rmap(new_page, vma, address);
		 
		new_page = old_page;
		mmu_notifier_invalidate_range_end(mm,
			address & huge_page_mask(h),
			(address & huge_page_mask(h)) + huge_page_size(h));
	}
	page_cache_release(new_page);
	page_cache_release(old_page);
	return 0;
}
