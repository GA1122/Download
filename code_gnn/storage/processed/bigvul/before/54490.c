static int remove_migration_pte(struct page *new, struct vm_area_struct *vma,
				 unsigned long addr, void *old)
{
	struct mm_struct *mm = vma->vm_mm;
	swp_entry_t entry;
 	pmd_t *pmd;
	pte_t *ptep, pte;
 	spinlock_t *ptl;

	if (unlikely(PageHuge(new))) {
		ptep = huge_pte_offset(mm, addr);
		if (!ptep)
			goto out;
		ptl = huge_pte_lockptr(hstate_vma(vma), mm, ptep);
	} else {
		pmd = mm_find_pmd(mm, addr);
		if (!pmd)
			goto out;

		ptep = pte_offset_map(pmd, addr);

		 

		ptl = pte_lockptr(mm, pmd);
	}

 	spin_lock(ptl);
	pte = *ptep;
	if (!is_swap_pte(pte))
		goto unlock;

	entry = pte_to_swp_entry(pte);

	if (!is_migration_entry(entry) ||
	    migration_entry_to_page(entry) != old)
		goto unlock;

	get_page(new);
	pte = pte_mkold(mk_pte(new, vma->vm_page_prot));
	if (pte_swp_soft_dirty(*ptep))
		pte = pte_mksoft_dirty(pte);

	 
	if (is_write_migration_entry(entry))
		pte = maybe_mkwrite(pte, vma);

#ifdef CONFIG_HUGETLB_PAGE
	if (PageHuge(new)) {
		pte = pte_mkhuge(pte);
		pte = arch_make_huge_pte(pte, vma, new, 0);
	}
#endif
	flush_dcache_page(new);
	set_pte_at(mm, addr, ptep, pte);

	if (PageHuge(new)) {
		if (PageAnon(new))
			hugepage_add_anon_rmap(new, vma, addr);
		else
			page_dup_rmap(new);
	} else if (PageAnon(new))
		page_add_anon_rmap(new, vma, addr);
	else
		page_add_file_rmap(new);

	if (vma->vm_flags & VM_LOCKED)
		mlock_vma_page(new);

	 
	update_mmu_cache(vma, addr, ptep);
unlock:
	pte_unmap_unlock(ptep, ptl);
out:
	return SWAP_AGAIN;
}