static void smaps_pmd_entry(pmd_t *pmd, unsigned long addr,
		struct mm_walk *walk)
{
	struct mem_size_stats *mss = walk->private;
	struct vm_area_struct *vma = walk->vma;
	bool locked = !!(vma->vm_flags & VM_LOCKED);
	struct page *page;

	 
	page = follow_trans_huge_pmd(vma, addr, pmd, FOLL_DUMP);
	if (IS_ERR_OR_NULL(page))
		return;
	if (PageAnon(page))
		mss->anonymous_thp += HPAGE_PMD_SIZE;
	else if (PageSwapBacked(page))
		mss->shmem_thp += HPAGE_PMD_SIZE;
	else if (is_zone_device_page(page))
		 ;
	else
		VM_BUG_ON_PAGE(1, page);
	smaps_account(mss, page, true, pmd_young(*pmd), pmd_dirty(*pmd), locked);
}
