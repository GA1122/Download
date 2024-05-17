int try_to_unmap_one(struct page *page, struct vm_area_struct *vma,
		     unsigned long address, void *arg)
{
	struct mm_struct *mm = vma->vm_mm;
	pte_t *pte;
	pte_t pteval;
	spinlock_t *ptl;
	int ret = SWAP_AGAIN;
	enum ttu_flags flags = (enum ttu_flags)arg;

	pte = page_check_address(page, mm, address, &ptl, 0);
	if (!pte)
		goto out;

	 
	if (!(flags & TTU_IGNORE_MLOCK)) {
		if (vma->vm_flags & VM_LOCKED)
			goto out_mlock;

		if (TTU_ACTION(flags) == TTU_MUNLOCK)
			goto out_unmap;
	}
	if (!(flags & TTU_IGNORE_ACCESS)) {
		if (ptep_clear_flush_young_notify(vma, address, pte)) {
			ret = SWAP_FAIL;
			goto out_unmap;
		}
  	}

	 
	flush_cache_page(vma, address, page_to_pfn(page));
	pteval = ptep_clear_flush(vma, address, pte);

	 
	if (pte_dirty(pteval))
		set_page_dirty(page);

	 
	update_hiwater_rss(mm);

	if (PageHWPoison(page) && !(flags & TTU_IGNORE_HWPOISON)) {
		if (!PageHuge(page)) {
			if (PageAnon(page))
				dec_mm_counter(mm, MM_ANONPAGES);
			else
				dec_mm_counter(mm, MM_FILEPAGES);
		}
		set_pte_at(mm, address, pte,
			   swp_entry_to_pte(make_hwpoison_entry(page)));
	} else if (pte_unused(pteval)) {
		 
		if (PageAnon(page))
			dec_mm_counter(mm, MM_ANONPAGES);
		else
			dec_mm_counter(mm, MM_FILEPAGES);
	} else if (PageAnon(page)) {
		swp_entry_t entry = { .val = page_private(page) };
		pte_t swp_pte;

		if (PageSwapCache(page)) {
			 
			if (swap_duplicate(entry) < 0) {
				set_pte_at(mm, address, pte, pteval);
				ret = SWAP_FAIL;
				goto out_unmap;
			}
			if (list_empty(&mm->mmlist)) {
				spin_lock(&mmlist_lock);
				if (list_empty(&mm->mmlist))
					list_add(&mm->mmlist, &init_mm.mmlist);
				spin_unlock(&mmlist_lock);
			}
			dec_mm_counter(mm, MM_ANONPAGES);
			inc_mm_counter(mm, MM_SWAPENTS);
		} else if (IS_ENABLED(CONFIG_MIGRATION)) {
			 
			BUG_ON(TTU_ACTION(flags) != TTU_MIGRATION);
			entry = make_migration_entry(page, pte_write(pteval));
		}
		swp_pte = swp_entry_to_pte(entry);
		if (pte_soft_dirty(pteval))
			swp_pte = pte_swp_mksoft_dirty(swp_pte);
		set_pte_at(mm, address, pte, swp_pte);
		BUG_ON(pte_file(*pte));
	} else if (IS_ENABLED(CONFIG_MIGRATION) &&
		   (TTU_ACTION(flags) == TTU_MIGRATION)) {
		 
		swp_entry_t entry;
		entry = make_migration_entry(page, pte_write(pteval));
		set_pte_at(mm, address, pte, swp_entry_to_pte(entry));
	} else
		dec_mm_counter(mm, MM_FILEPAGES);

	page_remove_rmap(page);
	page_cache_release(page);

out_unmap:
	pte_unmap_unlock(pte, ptl);
	if (ret != SWAP_FAIL)
		mmu_notifier_invalidate_page(mm, address);
out:
	return ret;

out_mlock:
	pte_unmap_unlock(pte, ptl);


	 
	if (down_read_trylock(&vma->vm_mm->mmap_sem)) {
		if (vma->vm_flags & VM_LOCKED) {
			mlock_vma_page(page);
			ret = SWAP_MLOCK;
		}
		up_read(&vma->vm_mm->mmap_sem);
	}
	return ret;
}
