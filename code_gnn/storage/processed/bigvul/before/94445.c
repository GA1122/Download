static int do_pmd_numa_page(struct mm_struct *mm, struct vm_area_struct *vma,
		     unsigned long addr, pmd_t *pmdp)
{
	pmd_t pmd;
	pte_t *pte, *orig_pte;
	unsigned long _addr = addr & PMD_MASK;
	unsigned long offset;
	spinlock_t *ptl;
	bool numa = false;
	int local_nid = numa_node_id();

	spin_lock(&mm->page_table_lock);
	pmd = *pmdp;
	if (pmd_numa(pmd)) {
		set_pmd_at(mm, _addr, pmdp, pmd_mknonnuma(pmd));
		numa = true;
	}
	spin_unlock(&mm->page_table_lock);

	if (!numa)
		return 0;

	 
	BUG_ON(!vma);
	BUG_ON(vma->vm_start >= _addr + PMD_SIZE);
	offset = max(_addr, vma->vm_start) & ~PMD_MASK;
	VM_BUG_ON(offset >= PMD_SIZE);
	orig_pte = pte = pte_offset_map_lock(mm, pmdp, _addr, &ptl);
	pte += offset >> PAGE_SHIFT;
	for (addr = _addr + offset; addr < _addr + PMD_SIZE; pte++, addr += PAGE_SIZE) {
		pte_t pteval = *pte;
		struct page *page;
		int curr_nid = local_nid;
		int target_nid;
		bool migrated;
		if (!pte_present(pteval))
			continue;
		if (!pte_numa(pteval))
			continue;
		if (addr >= vma->vm_end) {
			vma = find_vma(mm, addr);
			 
			BUG_ON(!vma);
			BUG_ON(addr < vma->vm_start);
		}
		if (pte_numa(pteval)) {
			pteval = pte_mknonnuma(pteval);
			set_pte_at(mm, addr, pte, pteval);
		}
		page = vm_normal_page(vma, addr, pteval);
		if (unlikely(!page))
			continue;
		 
		if (unlikely(page_mapcount(page) != 1))
			continue;

		 
		curr_nid = local_nid;
		target_nid = numa_migrate_prep(page, vma, addr,
					       page_to_nid(page));
		if (target_nid == -1) {
			put_page(page);
			continue;
		}

		 
		pte_unmap_unlock(pte, ptl);
		migrated = migrate_misplaced_page(page, target_nid);
		if (migrated)
			curr_nid = target_nid;
		task_numa_fault(curr_nid, 1, migrated);

		pte = pte_offset_map_lock(mm, pmdp, addr, &ptl);
	}
	pte_unmap_unlock(orig_pte, ptl);

	return 0;
}
