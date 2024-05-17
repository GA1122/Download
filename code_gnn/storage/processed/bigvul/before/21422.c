static int unuse_pte(struct vm_area_struct *vma, pmd_t *pmd,
		unsigned long addr, swp_entry_t entry, struct page *page)
{
	struct mem_cgroup *memcg;
	spinlock_t *ptl;
	pte_t *pte;
	int ret = 1;

	if (mem_cgroup_try_charge_swapin(vma->vm_mm, page,
					 GFP_KERNEL, &memcg)) {
		ret = -ENOMEM;
		goto out_nolock;
	}

	pte = pte_offset_map_lock(vma->vm_mm, pmd, addr, &ptl);
	if (unlikely(!pte_same(*pte, swp_entry_to_pte(entry)))) {
		if (ret > 0)
			mem_cgroup_cancel_charge_swapin(memcg);
		ret = 0;
		goto out;
	}

	dec_mm_counter(vma->vm_mm, MM_SWAPENTS);
	inc_mm_counter(vma->vm_mm, MM_ANONPAGES);
	get_page(page);
	set_pte_at(vma->vm_mm, addr, pte,
		   pte_mkold(mk_pte(page, vma->vm_page_prot)));
	page_add_anon_rmap(page, vma, addr);
	mem_cgroup_commit_charge_swapin(page, memcg);
	swap_free(entry);
	 
	activate_page(page);
out:
	pte_unmap_unlock(pte, ptl);
out_nolock:
	return ret;
}
