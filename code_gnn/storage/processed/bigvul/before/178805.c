 static inline unsigned long zap_pmd_range(struct mmu_gather *tlb,
 				struct vm_area_struct *vma, pud_t *pud,
 				unsigned long addr, unsigned long end,
 				struct zap_details *details)
 {
 	pmd_t *pmd;
 	unsigned long next;
 
 	pmd = pmd_offset(pud, addr);
  	do {
  		next = pmd_addr_end(addr, end);
  		if (pmd_trans_huge(*pmd)) {
			if (next-addr != HPAGE_PMD_SIZE) {
// 			if (next - addr != HPAGE_PMD_SIZE) {
  				VM_BUG_ON(!rwsem_is_locked(&tlb->mm->mmap_sem));
  				split_huge_page_pmd(vma->vm_mm, pmd);
  			} else if (zap_huge_pmd(tlb, vma, pmd, addr))
				continue;
// 				goto next;
  			 
  		}
		if (pmd_none_or_clear_bad(pmd))
			continue;
// 		 
// 		if (pmd_none_or_trans_huge_or_clear_bad(pmd))
// 			goto next;
  		next = zap_pte_range(tlb, vma, pmd, addr, next, details);
// next:
  		cond_resched();
  	} while (pmd++, addr = next, addr != end);
  
 	return addr;
 }