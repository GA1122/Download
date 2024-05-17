 static int mincore_pte_range(pmd_t *pmd, unsigned long addr, unsigned long end,
 			struct mm_walk *walk)
 {
 	spinlock_t *ptl;
 	struct vm_area_struct *vma = walk->vma;
 	pte_t *ptep;
 	unsigned char *vec = walk->private;
 	int nr = (end - addr) >> PAGE_SHIFT;
 
 	ptl = pmd_trans_huge_lock(pmd, vma);
 	if (ptl) {
 		memset(vec, 1, nr);
 		spin_unlock(ptl);
  		goto out;
  	}
  
// 	 
  	if (pmd_trans_unstable(pmd)) {
		__mincore_unmapped_range(addr, end, vma, vec);
// 		memset(vec, 1, nr);
  		goto out;
  	}
  
 	ptep = pte_offset_map_lock(walk->mm, pmd, addr, &ptl);
 	for (; addr != end; ptep++, addr += PAGE_SIZE) {
  		pte_t pte = *ptep;
  
  		if (pte_none(pte))
			__mincore_unmapped_range(addr, addr + PAGE_SIZE,
						 vma, vec);
// 			*vec = 0;
  		else if (pte_present(pte))
  			*vec = 1;
  		else {  
  			swp_entry_t entry = pte_to_swp_entry(pte);
  
			if (non_swap_entry(entry)) {
				 
				*vec = 1;
			} else {
#ifdef CONFIG_SWAP
				*vec = mincore_page(swap_address_space(entry),
						    swp_offset(entry));
#else
				WARN_ON(1);
				*vec = 1;
#endif
			}
// 			 
// 			*vec = !!non_swap_entry(entry);
  		}
  		vec++;
  	}
 	pte_unmap_unlock(ptep - 1, ptl);
 out:
 	walk->private += nr;
 	cond_resched();
 	return 0;
 }