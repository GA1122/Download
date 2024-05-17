static void __collapse_huge_page_copy(pte_t *pte, struct page *page,
				      struct vm_area_struct *vma,
				      unsigned long address,
				      spinlock_t *ptl)
{
	pte_t *_pte;
	for (_pte = pte; _pte < pte+HPAGE_PMD_NR; _pte++) {
		pte_t pteval = *_pte;
		struct page *src_page;

		if (pte_none(pteval)) {
			clear_user_highpage(page, address);
			add_mm_counter(vma->vm_mm, MM_ANONPAGES, 1);
		} else {
			src_page = pte_page(pteval);
			copy_user_highpage(page, src_page, address, vma);
			VM_BUG_ON(page_mapcount(src_page) != 1);
			VM_BUG_ON(page_count(src_page) != 2);
			release_pte_page(src_page);
			 
			spin_lock(ptl);
			 
			pte_clear(vma->vm_mm, address, _pte);
			page_remove_rmap(src_page);
			spin_unlock(ptl);
			free_page_and_swap_cache(src_page);
		}

		address += PAGE_SIZE;
		page++;
	}
}