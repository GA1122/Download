static int __do_fault(struct mm_struct *mm, struct vm_area_struct *vma,
		unsigned long address, pmd_t *pmd,
		pgoff_t pgoff, unsigned int flags, pte_t orig_pte)
{
	pte_t *page_table;
	spinlock_t *ptl;
	struct page *page;
	struct page *cow_page;
	pte_t entry;
	int anon = 0;
	struct page *dirty_page = NULL;
	struct vm_fault vmf;
	int ret;
	int page_mkwrite = 0;

	 
	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED)) {

		if (unlikely(anon_vma_prepare(vma)))
			return VM_FAULT_OOM;

		cow_page = alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, address);
		if (!cow_page)
			return VM_FAULT_OOM;

		if (mem_cgroup_newpage_charge(cow_page, mm, GFP_KERNEL)) {
			page_cache_release(cow_page);
			return VM_FAULT_OOM;
		}
	} else
		cow_page = NULL;

	vmf.virtual_address = (void __user *)(address & PAGE_MASK);
	vmf.pgoff = pgoff;
	vmf.flags = flags;
	vmf.page = NULL;

	ret = vma->vm_ops->fault(vma, &vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
			    VM_FAULT_RETRY)))
		goto uncharge_out;

	if (unlikely(PageHWPoison(vmf.page))) {
		if (ret & VM_FAULT_LOCKED)
			unlock_page(vmf.page);
		ret = VM_FAULT_HWPOISON;
		goto uncharge_out;
	}

	 
	if (unlikely(!(ret & VM_FAULT_LOCKED)))
		lock_page(vmf.page);
	else
		VM_BUG_ON(!PageLocked(vmf.page));

	 
	page = vmf.page;
	if (flags & FAULT_FLAG_WRITE) {
		if (!(vma->vm_flags & VM_SHARED)) {
			page = cow_page;
			anon = 1;
			copy_user_highpage(page, vmf.page, address, vma);
			__SetPageUptodate(page);
		} else {
			 
			if (vma->vm_ops->page_mkwrite) {
				int tmp;

				unlock_page(page);
				vmf.flags = FAULT_FLAG_WRITE|FAULT_FLAG_MKWRITE;
				tmp = vma->vm_ops->page_mkwrite(vma, &vmf);
				if (unlikely(tmp &
					  (VM_FAULT_ERROR | VM_FAULT_NOPAGE))) {
					ret = tmp;
					goto unwritable_page;
				}
				if (unlikely(!(tmp & VM_FAULT_LOCKED))) {
					lock_page(page);
					if (!page->mapping) {
						ret = 0;  
						unlock_page(page);
						goto unwritable_page;
					}
				} else
					VM_BUG_ON(!PageLocked(page));
				page_mkwrite = 1;
			}
		}

	}

	page_table = pte_offset_map_lock(mm, pmd, address, &ptl);

	 
	 
	if (likely(pte_same(*page_table, orig_pte))) {
		flush_icache_page(vma, page);
		entry = mk_pte(page, vma->vm_page_prot);
		if (flags & FAULT_FLAG_WRITE)
			entry = maybe_mkwrite(pte_mkdirty(entry), vma);
		if (anon) {
			inc_mm_counter_fast(mm, MM_ANONPAGES);
			page_add_new_anon_rmap(page, vma, address);
		} else {
			inc_mm_counter_fast(mm, MM_FILEPAGES);
			page_add_file_rmap(page);
			if (flags & FAULT_FLAG_WRITE) {
				dirty_page = page;
				get_page(dirty_page);
			}
		}
		set_pte_at(mm, address, page_table, entry);

		 
		update_mmu_cache(vma, address, page_table);
	} else {
		if (cow_page)
			mem_cgroup_uncharge_page(cow_page);
		if (anon)
			page_cache_release(page);
		else
			anon = 1;  
	}

	pte_unmap_unlock(page_table, ptl);

	if (dirty_page) {
		struct address_space *mapping = page->mapping;

		if (set_page_dirty(dirty_page))
			page_mkwrite = 1;
		unlock_page(dirty_page);
		put_page(dirty_page);
		if (page_mkwrite && mapping) {
			 
			balance_dirty_pages_ratelimited(mapping);
		}

		 
		if (vma->vm_file)
			file_update_time(vma->vm_file);
	} else {
		unlock_page(vmf.page);
		if (anon)
			page_cache_release(vmf.page);
	}

	return ret;

unwritable_page:
	page_cache_release(page);
	return ret;
uncharge_out:
	 
	if (cow_page) {
		mem_cgroup_uncharge_page(cow_page);
		page_cache_release(cow_page);
	}
	return ret;
}