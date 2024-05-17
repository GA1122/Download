static int do_cow_fault(struct mm_struct *mm, struct vm_area_struct *vma,
		unsigned long address, pmd_t *pmd,
		pgoff_t pgoff, unsigned int flags, pte_t orig_pte)
{
	struct page *fault_page, *new_page;
	struct mem_cgroup *memcg;
	spinlock_t *ptl;
	pte_t *pte;
	int ret;

	if (unlikely(anon_vma_prepare(vma)))
		return VM_FAULT_OOM;

	new_page = alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, address);
	if (!new_page)
		return VM_FAULT_OOM;

	if (mem_cgroup_try_charge(new_page, mm, GFP_KERNEL, &memcg)) {
		page_cache_release(new_page);
		return VM_FAULT_OOM;
	}

	ret = __do_fault(vma, address, pgoff, flags, new_page, &fault_page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		goto uncharge_out;

	if (fault_page)
		copy_user_highpage(new_page, fault_page, address, vma);
	__SetPageUptodate(new_page);

	pte = pte_offset_map_lock(mm, pmd, address, &ptl);
	if (unlikely(!pte_same(*pte, orig_pte))) {
		pte_unmap_unlock(pte, ptl);
		if (fault_page) {
			unlock_page(fault_page);
			page_cache_release(fault_page);
		} else {
			 
			i_mmap_unlock_read(vma->vm_file->f_mapping);
		}
		goto uncharge_out;
	}
	do_set_pte(vma, address, new_page, pte, true, true);
	mem_cgroup_commit_charge(new_page, memcg, false);
	lru_cache_add_active_or_unevictable(new_page, vma);
	pte_unmap_unlock(pte, ptl);
	if (fault_page) {
		unlock_page(fault_page);
		page_cache_release(fault_page);
	} else {
		 
		i_mmap_unlock_read(vma->vm_file->f_mapping);
	}
	return ret;
uncharge_out:
	mem_cgroup_cancel_charge(new_page, memcg);
	page_cache_release(new_page);
	return ret;
}