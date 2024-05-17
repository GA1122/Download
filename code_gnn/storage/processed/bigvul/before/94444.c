int do_numa_page(struct mm_struct *mm, struct vm_area_struct *vma,
		   unsigned long addr, pte_t pte, pte_t *ptep, pmd_t *pmd)
{
	struct page *page = NULL;
	spinlock_t *ptl;
	int current_nid = -1;
	int target_nid;
	bool migrated = false;

	 
	ptl = pte_lockptr(mm, pmd);
	spin_lock(ptl);
	if (unlikely(!pte_same(*ptep, pte))) {
		pte_unmap_unlock(ptep, ptl);
		goto out;
	}

	pte = pte_mknonnuma(pte);
	set_pte_at(mm, addr, ptep, pte);
	update_mmu_cache(vma, addr, ptep);

	page = vm_normal_page(vma, addr, pte);
	if (!page) {
		pte_unmap_unlock(ptep, ptl);
		return 0;
	}

	current_nid = page_to_nid(page);
	target_nid = numa_migrate_prep(page, vma, addr, current_nid);
	pte_unmap_unlock(ptep, ptl);
	if (target_nid == -1) {
		 
		current_nid = numa_node_id();
		put_page(page);
		goto out;
	}

	 
	migrated = migrate_misplaced_page(page, target_nid);
	if (migrated)
		current_nid = target_nid;

out:
	if (current_nid != -1)
		task_numa_fault(current_nid, 1, migrated);
	return 0;
}