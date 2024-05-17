 int hugetlb_mcopy_atomic_pte(struct mm_struct *dst_mm,
 			    pte_t *dst_pte,
 			    struct vm_area_struct *dst_vma,
 			    unsigned long dst_addr,
  			    unsigned long src_addr,
  			    struct page **pagep)
  {
// 	struct address_space *mapping;
// 	pgoff_t idx;
// 	unsigned long size;
  	int vm_shared = dst_vma->vm_flags & VM_SHARED;
  	struct hstate *h = hstate_vma(dst_vma);
  	pte_t _dst_pte;
 	spinlock_t *ptl;
 	int ret;
 	struct page *page;
 
 	if (!*pagep) {
 		ret = -ENOMEM;
 		page = alloc_huge_page(dst_vma, dst_addr, 0);
 		if (IS_ERR(page))
 			goto out;
 
 		ret = copy_huge_page_from_user(page,
 						(const void __user *) src_addr,
 						pages_per_huge_page(h), false);
 
 		 
 		if (unlikely(ret)) {
 			ret = -EFAULT;
 			*pagep = page;
 			 
 			goto out;
 		}
 	} else {
 		page = *pagep;
 		*pagep = NULL;
 	}
 
 	 
  	__SetPageUptodate(page);
  	set_page_huge_active(page);
  
// 	mapping = dst_vma->vm_file->f_mapping;
// 	idx = vma_hugecache_offset(h, dst_vma, dst_addr);
// 
  	 
  	if (vm_shared) {
		struct address_space *mapping = dst_vma->vm_file->f_mapping;
		pgoff_t idx = vma_hugecache_offset(h, dst_vma, dst_addr);
// 		size = i_size_read(mapping->host) >> huge_page_shift(h);
// 		ret = -EFAULT;
// 		if (idx >= size)
// 			goto out_release_nounlock;
  
// 		 
  		ret = huge_add_to_page_cache(page, mapping, idx);
  		if (ret)
  			goto out_release_nounlock;
 	}
 
  	ptl = huge_pte_lockptr(h, dst_mm, dst_pte);
  	spin_lock(ptl);
  
// 	 
// 	size = i_size_read(mapping->host) >> huge_page_shift(h);
// 	ret = -EFAULT;
// 	if (idx >= size)
// 		goto out_release_unlock;
// 
  	ret = -EEXIST;
  	if (!huge_pte_none(huge_ptep_get(dst_pte)))
  		goto out_release_unlock;
 
 	if (vm_shared) {
 		page_dup_rmap(page, true);
 	} else {
 		ClearPagePrivate(page);
 		hugepage_add_new_anon_rmap(page, dst_vma, dst_addr);
 	}
 
 	_dst_pte = make_huge_pte(dst_vma, page, dst_vma->vm_flags & VM_WRITE);
 	if (dst_vma->vm_flags & VM_WRITE)
 		_dst_pte = huge_pte_mkdirty(_dst_pte);
 	_dst_pte = pte_mkyoung(_dst_pte);
 
 	set_huge_pte_at(dst_mm, dst_addr, dst_pte, _dst_pte);
 
 	(void)huge_ptep_set_access_flags(dst_vma, dst_addr, dst_pte, _dst_pte,
 					dst_vma->vm_flags & VM_WRITE);
 	hugetlb_count_add(pages_per_huge_page(h), dst_mm);
 
 	 
 	update_mmu_cache(dst_vma, dst_addr, dst_pte);
 
 	spin_unlock(ptl);
 	if (vm_shared)
 		unlock_page(page);
 	ret = 0;
 out:
 	return ret;
 out_release_unlock:
 	spin_unlock(ptl);
 	if (vm_shared)
 		unlock_page(page);
 out_release_nounlock:
 	put_page(page);
 	goto out;
 }