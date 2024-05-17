 static int pagemap_pte_range(pmd_t *pmd, unsigned long addr, unsigned long end,
 			     struct mm_walk *walk)
 {
 	struct vm_area_struct *vma;
 	struct pagemapread *pm = walk->private;
 	pte_t *pte;
  	int err = 0;
  
  	split_huge_page_pmd(walk->mm, pmd);
// 	if (pmd_trans_unstable(pmd))
// 		return 0;
  
  	 
  	vma = find_vma(walk->mm, addr);
 	for (; addr != end; addr += PAGE_SIZE) {
 		u64 pfn = PM_NOT_PRESENT;
 
 		 
 		if (vma && (addr >= vma->vm_end))
 			vma = find_vma(walk->mm, addr);
 
 		 
 		if (vma && (vma->vm_start <= addr) &&
 		    !is_vm_hugetlb_page(vma)) {
 			pte = pte_offset_map(pmd, addr);
 			pfn = pte_to_pagemap_entry(*pte);
 			 
 			pte_unmap(pte);
 		}
 		err = add_to_pagemap(addr, pfn, pm);
 		if (err)
 			return err;
 	}
 
 	cond_resched();
 
 	return err;
 }