static int hugetlb_cow(struct mm_struct *mm, struct vm_area_struct *vma,
		       unsigned long address, pte_t *ptep,
		       struct page *pagecache_page, spinlock_t *ptl)
{
	pte_t pte;
	struct hstate *h = hstate_vma(vma);
	struct page *old_page, *new_page;
	int ret = 0, outside_reserve = 0;
	unsigned long mmun_start;	 
	unsigned long mmun_end;		 

	pte = huge_ptep_get(ptep);
	old_page = pte_page(pte);

retry_avoidcopy:
	 
	if (page_mapcount(old_page) == 1 && PageAnon(old_page)) {
		page_move_anon_rmap(old_page, vma);
		set_huge_ptep_writable(vma, address, ptep);
		return 0;
	}

	 
	if (is_vma_resv_set(vma, HPAGE_RESV_OWNER) &&
			old_page != pagecache_page)
		outside_reserve = 1;

	get_page(old_page);

	 
	spin_unlock(ptl);
	new_page = alloc_huge_page(vma, address, outside_reserve);

	if (IS_ERR(new_page)) {
		 
		if (outside_reserve) {
			put_page(old_page);
			BUG_ON(huge_pte_none(pte));
			unmap_ref_private(mm, vma, old_page, address);
			BUG_ON(huge_pte_none(pte));
			spin_lock(ptl);
			ptep = huge_pte_offset(mm, address & huge_page_mask(h),
					       huge_page_size(h));
			if (likely(ptep &&
				   pte_same(huge_ptep_get(ptep), pte)))
				goto retry_avoidcopy;
			 
			return 0;
		}

		ret = (PTR_ERR(new_page) == -ENOMEM) ?
			VM_FAULT_OOM : VM_FAULT_SIGBUS;
		goto out_release_old;
	}

	 
	if (unlikely(anon_vma_prepare(vma))) {
		ret = VM_FAULT_OOM;
		goto out_release_all;
	}

	copy_user_huge_page(new_page, old_page, address, vma,
			    pages_per_huge_page(h));
	__SetPageUptodate(new_page);
	set_page_huge_active(new_page);

	mmun_start = address & huge_page_mask(h);
	mmun_end = mmun_start + huge_page_size(h);
	mmu_notifier_invalidate_range_start(mm, mmun_start, mmun_end);

	 
	spin_lock(ptl);
	ptep = huge_pte_offset(mm, address & huge_page_mask(h),
			       huge_page_size(h));
	if (likely(ptep && pte_same(huge_ptep_get(ptep), pte))) {
		ClearPagePrivate(new_page);

		 
		huge_ptep_clear_flush(vma, address, ptep);
		mmu_notifier_invalidate_range(mm, mmun_start, mmun_end);
		set_huge_pte_at(mm, address, ptep,
				make_huge_pte(vma, new_page, 1));
		page_remove_rmap(old_page, true);
		hugepage_add_new_anon_rmap(new_page, vma, address);
		 
		new_page = old_page;
	}
	spin_unlock(ptl);
	mmu_notifier_invalidate_range_end(mm, mmun_start, mmun_end);
out_release_all:
	restore_reserve_on_error(h, vma, address, new_page);
	put_page(new_page);
out_release_old:
	put_page(old_page);

	spin_lock(ptl);  
	return ret;
}