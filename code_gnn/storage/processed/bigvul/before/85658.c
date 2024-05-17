static unsigned long __munlock_pagevec_fill(struct pagevec *pvec,
		struct vm_area_struct *vma, int zoneid,	unsigned long start,
		unsigned long end)
{
	pte_t *pte;
	spinlock_t *ptl;

	 
	pte = get_locked_pte(vma->vm_mm, start,	&ptl);
	 
	end = pgd_addr_end(start, end);
	end = p4d_addr_end(start, end);
	end = pud_addr_end(start, end);
	end = pmd_addr_end(start, end);

	 
	start += PAGE_SIZE;
	while (start < end) {
		struct page *page = NULL;
		pte++;
		if (pte_present(*pte))
			page = vm_normal_page(vma, start, *pte);
		 
		if (!page || page_zone_id(page) != zoneid)
			break;

		 
		if (PageTransCompound(page))
			break;

		get_page(page);
		 
		start += PAGE_SIZE;
		if (pagevec_add(pvec, page) == 0)
			break;
	}
	pte_unmap_unlock(pte, ptl);
	return start;
}
