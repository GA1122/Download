 static struct rmap_item *scan_get_next_rmap_item(struct page **page)
 {
 	struct mm_struct *mm;
 	struct mm_slot *slot;
 	struct vm_area_struct *vma;
 	struct rmap_item *rmap_item;
 
 	if (list_empty(&ksm_mm_head.mm_list))
 		return NULL;
 
 	slot = ksm_scan.mm_slot;
 	if (slot == &ksm_mm_head) {
 		 
 		lru_add_drain_all();
 
 		root_unstable_tree = RB_ROOT;
 
 		spin_lock(&ksm_mmlist_lock);
  		slot = list_entry(slot->mm_list.next, struct mm_slot, mm_list);
  		ksm_scan.mm_slot = slot;
  		spin_unlock(&ksm_mmlist_lock);
// 		 
// 		if (slot == &ksm_mm_head)
// 			return NULL;
  next_mm:
  		ksm_scan.address = 0;
  		ksm_scan.rmap_list = &slot->rmap_list;
 	}
 
 	mm = slot->mm;
 	down_read(&mm->mmap_sem);
 	if (ksm_test_exit(mm))
 		vma = NULL;
 	else
 		vma = find_vma(mm, ksm_scan.address);
 
 	for (; vma; vma = vma->vm_next) {
 		if (!(vma->vm_flags & VM_MERGEABLE))
 			continue;
 		if (ksm_scan.address < vma->vm_start)
 			ksm_scan.address = vma->vm_start;
 		if (!vma->anon_vma)
 			ksm_scan.address = vma->vm_end;
 
 		while (ksm_scan.address < vma->vm_end) {
 			if (ksm_test_exit(mm))
 				break;
 			*page = follow_page(vma, ksm_scan.address, FOLL_GET);
 			if (IS_ERR_OR_NULL(*page)) {
 				ksm_scan.address += PAGE_SIZE;
 				cond_resched();
 				continue;
 			}
 			if (PageAnon(*page) ||
 			    page_trans_compound_anon(*page)) {
 				flush_anon_page(vma, *page, ksm_scan.address);
 				flush_dcache_page(*page);
 				rmap_item = get_next_rmap_item(slot,
 					ksm_scan.rmap_list, ksm_scan.address);
 				if (rmap_item) {
 					ksm_scan.rmap_list =
 							&rmap_item->rmap_list;
 					ksm_scan.address += PAGE_SIZE;
 				} else
 					put_page(*page);
 				up_read(&mm->mmap_sem);
 				return rmap_item;
 			}
 			put_page(*page);
 			ksm_scan.address += PAGE_SIZE;
 			cond_resched();
 		}
 	}
 
 	if (ksm_test_exit(mm)) {
 		ksm_scan.address = 0;
 		ksm_scan.rmap_list = &slot->rmap_list;
 	}
 	 
 	remove_trailing_rmap_items(slot, ksm_scan.rmap_list);
 
 	spin_lock(&ksm_mmlist_lock);
 	ksm_scan.mm_slot = list_entry(slot->mm_list.next,
 						struct mm_slot, mm_list);
 	if (ksm_scan.address == 0) {
 		 
 		hlist_del(&slot->link);
 		list_del(&slot->mm_list);
 		spin_unlock(&ksm_mmlist_lock);
 
 		free_mm_slot(slot);
 		clear_bit(MMF_VM_MERGEABLE, &mm->flags);
 		up_read(&mm->mmap_sem);
 		mmdrop(mm);
 	} else {
 		spin_unlock(&ksm_mmlist_lock);
 		up_read(&mm->mmap_sem);
 	}
 
 	 
 	slot = ksm_scan.mm_slot;
 	if (slot != &ksm_mm_head)
 		goto next_mm;
 
 	ksm_scan.seqnr++;
 	return NULL;
 }