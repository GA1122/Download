 static struct page *follow_pmd_mask(struct vm_area_struct *vma,
 				    unsigned long address, pud_t *pudp,
 				    unsigned int flags,
 				    struct follow_page_context *ctx)
 {
 	pmd_t *pmd, pmdval;
 	spinlock_t *ptl;
 	struct page *page;
 	struct mm_struct *mm = vma->vm_mm;
 
 	pmd = pmd_offset(pudp, address);
 	 
 	pmdval = READ_ONCE(*pmd);
 	if (pmd_none(pmdval))
 		return no_page_table(vma, flags);
 	if (pmd_huge(pmdval) && vma->vm_flags & VM_HUGETLB) {
 		page = follow_huge_pmd(mm, address, pmd, flags);
 		if (page)
 			return page;
 		return no_page_table(vma, flags);
 	}
 	if (is_hugepd(__hugepd(pmd_val(pmdval)))) {
 		page = follow_huge_pd(vma, address,
 				      __hugepd(pmd_val(pmdval)), flags,
 				      PMD_SHIFT);
 		if (page)
 			return page;
 		return no_page_table(vma, flags);
 	}
 retry:
 	if (!pmd_present(pmdval)) {
 		if (likely(!(flags & FOLL_MIGRATION)))
 			return no_page_table(vma, flags);
 		VM_BUG_ON(thp_migration_supported() &&
 				  !is_pmd_migration_entry(pmdval));
 		if (is_pmd_migration_entry(pmdval))
 			pmd_migration_entry_wait(mm, pmd);
 		pmdval = READ_ONCE(*pmd);
 		 
 		if (pmd_none(pmdval))
 			return no_page_table(vma, flags);
 		goto retry;
 	}
 	if (pmd_devmap(pmdval)) {
 		ptl = pmd_lock(mm, pmd);
 		page = follow_devmap_pmd(vma, address, pmd, flags, &ctx->pgmap);
 		spin_unlock(ptl);
 		if (page)
 			return page;
 	}
 	if (likely(!pmd_trans_huge(pmdval)))
 		return follow_page_pte(vma, address, pmd, flags, &ctx->pgmap);
 
 	if ((flags & FOLL_NUMA) && pmd_protnone(pmdval))
 		return no_page_table(vma, flags);
 
 retry_locked:
 	ptl = pmd_lock(mm, pmd);
 	if (unlikely(pmd_none(*pmd))) {
 		spin_unlock(ptl);
 		return no_page_table(vma, flags);
 	}
 	if (unlikely(!pmd_present(*pmd))) {
 		spin_unlock(ptl);
 		if (likely(!(flags & FOLL_MIGRATION)))
 			return no_page_table(vma, flags);
 		pmd_migration_entry_wait(mm, pmd);
 		goto retry_locked;
 	}
 	if (unlikely(!pmd_trans_huge(*pmd))) {
 		spin_unlock(ptl);
 		return follow_page_pte(vma, address, pmd, flags, &ctx->pgmap);
 	}
 	if (flags & FOLL_SPLIT) {
 		int ret;
 		page = pmd_page(*pmd);
 		if (is_huge_zero_page(page)) {
 			spin_unlock(ptl);
 			ret = 0;
 			split_huge_pmd(vma, pmd, address);
  			if (pmd_trans_unstable(pmd))
  				ret = -EBUSY;
  		} else {
			get_page(page);
// 			if (unlikely(!try_get_page(page))) {
// 				spin_unlock(ptl);
// 				return ERR_PTR(-ENOMEM);
// 			}
  			spin_unlock(ptl);
  			lock_page(page);
  			ret = split_huge_page(page);
 			unlock_page(page);
 			put_page(page);
 			if (pmd_none(*pmd))
 				return no_page_table(vma, flags);
 		}
 
 		return ret ? ERR_PTR(ret) :
 			follow_page_pte(vma, address, pmd, flags, &ctx->pgmap);
 	}
 	page = follow_trans_huge_pmd(vma, address, pmd, flags);
 	spin_unlock(ptl);
 	ctx->page_mask = HPAGE_PMD_NR - 1;
 	return page;
 }