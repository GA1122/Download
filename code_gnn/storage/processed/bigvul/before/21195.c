	__releases(ptl)
{
	struct page *old_page, *new_page;
	pte_t entry;
	int ret = 0;
	int page_mkwrite = 0;
	struct page *dirty_page = NULL;

	old_page = vm_normal_page(vma, address, orig_pte);
	if (!old_page) {
		 
		if ((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
				     (VM_WRITE|VM_SHARED))
			goto reuse;
		goto gotten;
	}

	 
	if (PageAnon(old_page) && !PageKsm(old_page)) {
		if (!trylock_page(old_page)) {
			page_cache_get(old_page);
			pte_unmap_unlock(page_table, ptl);
			lock_page(old_page);
			page_table = pte_offset_map_lock(mm, pmd, address,
							 &ptl);
			if (!pte_same(*page_table, orig_pte)) {
				unlock_page(old_page);
				goto unlock;
			}
			page_cache_release(old_page);
		}
		if (reuse_swap_page(old_page)) {
			 
			page_move_anon_rmap(old_page, vma, address);
			unlock_page(old_page);
			goto reuse;
		}
		unlock_page(old_page);
	} else if (unlikely((vma->vm_flags & (VM_WRITE|VM_SHARED)) ==
					(VM_WRITE|VM_SHARED))) {
		 
		if (vma->vm_ops && vma->vm_ops->page_mkwrite) {
			struct vm_fault vmf;
			int tmp;

			vmf.virtual_address = (void __user *)(address &
								PAGE_MASK);
			vmf.pgoff = old_page->index;
			vmf.flags = FAULT_FLAG_WRITE|FAULT_FLAG_MKWRITE;
			vmf.page = old_page;

			 
			page_cache_get(old_page);
			pte_unmap_unlock(page_table, ptl);

			tmp = vma->vm_ops->page_mkwrite(vma, &vmf);
			if (unlikely(tmp &
					(VM_FAULT_ERROR | VM_FAULT_NOPAGE))) {
				ret = tmp;
				goto unwritable_page;
			}
			if (unlikely(!(tmp & VM_FAULT_LOCKED))) {
				lock_page(old_page);
				if (!old_page->mapping) {
					ret = 0;  
					unlock_page(old_page);
					goto unwritable_page;
				}
			} else
				VM_BUG_ON(!PageLocked(old_page));

			 
			page_table = pte_offset_map_lock(mm, pmd, address,
							 &ptl);
			if (!pte_same(*page_table, orig_pte)) {
				unlock_page(old_page);
				goto unlock;
			}

			page_mkwrite = 1;
		}
		dirty_page = old_page;
		get_page(dirty_page);

reuse:
		flush_cache_page(vma, address, pte_pfn(orig_pte));
		entry = pte_mkyoung(orig_pte);
		entry = maybe_mkwrite(pte_mkdirty(entry), vma);
		if (ptep_set_access_flags(vma, address, page_table, entry,1))
			update_mmu_cache(vma, address, page_table);
		pte_unmap_unlock(page_table, ptl);
		ret |= VM_FAULT_WRITE;

		if (!dirty_page)
			return ret;

		 
		if (!page_mkwrite) {
			wait_on_page_locked(dirty_page);
			set_page_dirty_balance(dirty_page, page_mkwrite);
		}
		put_page(dirty_page);
		if (page_mkwrite) {
			struct address_space *mapping = dirty_page->mapping;

			set_page_dirty(dirty_page);
			unlock_page(dirty_page);
			page_cache_release(dirty_page);
			if (mapping)	{
				 
				balance_dirty_pages_ratelimited(mapping);
			}
		}

		 
		if (vma->vm_file)
			file_update_time(vma->vm_file);

		return ret;
	}

	 
	page_cache_get(old_page);
gotten:
	pte_unmap_unlock(page_table, ptl);

	if (unlikely(anon_vma_prepare(vma)))
		goto oom;

	if (is_zero_pfn(pte_pfn(orig_pte))) {
		new_page = alloc_zeroed_user_highpage_movable(vma, address);
		if (!new_page)
			goto oom;
	} else {
		new_page = alloc_page_vma(GFP_HIGHUSER_MOVABLE, vma, address);
		if (!new_page)
			goto oom;
		cow_user_page(new_page, old_page, address, vma);
	}
	__SetPageUptodate(new_page);

	if (mem_cgroup_newpage_charge(new_page, mm, GFP_KERNEL))
		goto oom_free_new;

	 
	page_table = pte_offset_map_lock(mm, pmd, address, &ptl);
	if (likely(pte_same(*page_table, orig_pte))) {
		if (old_page) {
			if (!PageAnon(old_page)) {
				dec_mm_counter_fast(mm, MM_FILEPAGES);
				inc_mm_counter_fast(mm, MM_ANONPAGES);
			}
		} else
			inc_mm_counter_fast(mm, MM_ANONPAGES);
		flush_cache_page(vma, address, pte_pfn(orig_pte));
		entry = mk_pte(new_page, vma->vm_page_prot);
		entry = maybe_mkwrite(pte_mkdirty(entry), vma);
		 
		ptep_clear_flush(vma, address, page_table);
		page_add_new_anon_rmap(new_page, vma, address);
		 
		set_pte_at_notify(mm, address, page_table, entry);
		update_mmu_cache(vma, address, page_table);
		if (old_page) {
			 
			page_remove_rmap(old_page);
		}

		 
		new_page = old_page;
		ret |= VM_FAULT_WRITE;
	} else
		mem_cgroup_uncharge_page(new_page);

	if (new_page)
		page_cache_release(new_page);
unlock:
	pte_unmap_unlock(page_table, ptl);
	if (old_page) {
		 
		if ((ret & VM_FAULT_WRITE) && (vma->vm_flags & VM_LOCKED)) {
			lock_page(old_page);	 
			munlock_vma_page(old_page);
			unlock_page(old_page);
		}
		page_cache_release(old_page);
	}
	return ret;
oom_free_new:
	page_cache_release(new_page);
oom:
	if (old_page) {
		if (page_mkwrite) {
			unlock_page(old_page);
			page_cache_release(old_page);
		}
		page_cache_release(old_page);
	}
	return VM_FAULT_OOM;

unwritable_page:
	page_cache_release(old_page);
	return ret;
}
