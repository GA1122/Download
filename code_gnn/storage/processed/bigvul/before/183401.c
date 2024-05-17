 long follow_hugetlb_page(struct mm_struct *mm, struct vm_area_struct *vma,
 			 struct page **pages, struct vm_area_struct **vmas,
 			 unsigned long *position, unsigned long *nr_pages,
 			 long i, unsigned int flags, int *nonblocking)
 {
 	unsigned long pfn_offset;
 	unsigned long vaddr = *position;
 	unsigned long remainder = *nr_pages;
 	struct hstate *h = hstate_vma(vma);
 	int err = -EFAULT;
 
 	while (vaddr < vma->vm_end && remainder) {
 		pte_t *pte;
 		spinlock_t *ptl = NULL;
 		int absent;
 		struct page *page;
 
 		 
 		if (fatal_signal_pending(current)) {
 			remainder = 0;
 			break;
 		}
 
 		 
 		pte = huge_pte_offset(mm, vaddr & huge_page_mask(h),
 				      huge_page_size(h));
 		if (pte)
 			ptl = huge_pte_lock(h, mm, pte);
 		absent = !pte || huge_pte_none(huge_ptep_get(pte));
 
 		 
 		if (absent && (flags & FOLL_DUMP) &&
 		    !hugetlbfs_pagecache_present(h, vma, vaddr)) {
 			if (pte)
 				spin_unlock(ptl);
 			remainder = 0;
 			break;
 		}
 
 		 
 		if (absent || is_swap_pte(huge_ptep_get(pte)) ||
 		    ((flags & FOLL_WRITE) &&
 		      !huge_pte_write(huge_ptep_get(pte)))) {
 			vm_fault_t ret;
 			unsigned int fault_flags = 0;
 
 			if (pte)
 				spin_unlock(ptl);
 			if (flags & FOLL_WRITE)
 				fault_flags |= FAULT_FLAG_WRITE;
 			if (nonblocking)
 				fault_flags |= FAULT_FLAG_ALLOW_RETRY;
 			if (flags & FOLL_NOWAIT)
 				fault_flags |= FAULT_FLAG_ALLOW_RETRY |
 					FAULT_FLAG_RETRY_NOWAIT;
 			if (flags & FOLL_TRIED) {
 				VM_WARN_ON_ONCE(fault_flags &
 						FAULT_FLAG_ALLOW_RETRY);
 				fault_flags |= FAULT_FLAG_TRIED;
 			}
 			ret = hugetlb_fault(mm, vma, vaddr, fault_flags);
 			if (ret & VM_FAULT_ERROR) {
 				err = vm_fault_to_errno(ret, flags);
 				remainder = 0;
 				break;
 			}
 			if (ret & VM_FAULT_RETRY) {
 				if (nonblocking &&
 				    !(fault_flags & FAULT_FLAG_RETRY_NOWAIT))
 					*nonblocking = 0;
 				*nr_pages = 0;
 				 
 				return i;
 			}
 			continue;
 		}
  
  		pfn_offset = (vaddr & ~huge_page_mask(h)) >> PAGE_SHIFT;
  		page = pte_page(huge_ptep_get(pte));
// 
// 		 
// 		if (unlikely(page_count(page) <= 0)) {
// 			if (pages) {
// 				spin_unlock(ptl);
// 				remainder = 0;
// 				err = -ENOMEM;
// 				break;
// 			}
// 		}
  same_page:
  		if (pages) {
  			pages[i] = mem_map_offset(page, pfn_offset);
 			get_page(pages[i]);
 		}
 
 		if (vmas)
 			vmas[i] = vma;
 
 		vaddr += PAGE_SIZE;
 		++pfn_offset;
 		--remainder;
 		++i;
 		if (vaddr < vma->vm_end && remainder &&
 				pfn_offset < pages_per_huge_page(h)) {
 			 
 			goto same_page;
 		}
 		spin_unlock(ptl);
 	}
 	*nr_pages = remainder;
 	 
 	*position = vaddr;
 
 	return i ? i : err;
 }