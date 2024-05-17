int page_referenced_one(struct page *page, struct vm_area_struct *vma,
			unsigned long address, void *arg)
{
	struct mm_struct *mm = vma->vm_mm;
	spinlock_t *ptl;
	int referenced = 0;
	struct page_referenced_arg *pra = arg;

	if (unlikely(PageTransHuge(page))) {
		pmd_t *pmd;

		 
		pmd = page_check_address_pmd(page, mm, address,
					     PAGE_CHECK_ADDRESS_PMD_FLAG, &ptl);
		if (!pmd)
			return SWAP_AGAIN;

		if (vma->vm_flags & VM_LOCKED) {
			spin_unlock(ptl);
			pra->vm_flags |= VM_LOCKED;
			return SWAP_FAIL;  
		}

		 
		if (pmdp_clear_flush_young_notify(vma, address, pmd))
			referenced++;
		spin_unlock(ptl);
	} else {
		pte_t *pte;

		 
		pte = page_check_address(page, mm, address, &ptl, 0);
		if (!pte)
			return SWAP_AGAIN;

		if (vma->vm_flags & VM_LOCKED) {
			pte_unmap_unlock(pte, ptl);
			pra->vm_flags |= VM_LOCKED;
			return SWAP_FAIL;  
		}

		if (ptep_clear_flush_young_notify(vma, address, pte)) {
			 
			if (likely(!(vma->vm_flags & VM_SEQ_READ)))
				referenced++;
		}
		pte_unmap_unlock(pte, ptl);
	}

	if (referenced) {
		pra->referenced++;
		pra->vm_flags |= vma->vm_flags;
	}

	pra->mapcount--;
	if (!pra->mapcount)
		return SWAP_SUCCESS;  

	return SWAP_AGAIN;
}
