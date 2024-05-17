static int hugetlb_no_page(struct mm_struct *mm, struct vm_area_struct *vma,
			   struct address_space *mapping, pgoff_t idx,
			   unsigned long address, pte_t *ptep, unsigned int flags)
{
	struct hstate *h = hstate_vma(vma);
	int ret = VM_FAULT_SIGBUS;
	int anon_rmap = 0;
	unsigned long size;
	struct page *page;
	pte_t new_pte;
	spinlock_t *ptl;

	 
	if (is_vma_resv_set(vma, HPAGE_RESV_UNMAPPED)) {
		pr_warn_ratelimited("PID %d killed due to inadequate hugepage pool\n",
			   current->pid);
		return ret;
	}

	 
retry:
	page = find_lock_page(mapping, idx);
	if (!page) {
		size = i_size_read(mapping->host) >> huge_page_shift(h);
		if (idx >= size)
			goto out;

		 
		if (userfaultfd_missing(vma)) {
			u32 hash;
			struct vm_fault vmf = {
				.vma = vma,
				.address = address,
				.flags = flags,
				 
			};

			 
			hash = hugetlb_fault_mutex_hash(h, mm, vma, mapping,
							idx, address);
			mutex_unlock(&hugetlb_fault_mutex_table[hash]);
			ret = handle_userfault(&vmf, VM_UFFD_MISSING);
			mutex_lock(&hugetlb_fault_mutex_table[hash]);
			goto out;
		}

		page = alloc_huge_page(vma, address, 0);
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			if (ret == -ENOMEM)
				ret = VM_FAULT_OOM;
			else
				ret = VM_FAULT_SIGBUS;
			goto out;
		}
		clear_huge_page(page, address, pages_per_huge_page(h));
		__SetPageUptodate(page);
		set_page_huge_active(page);

		if (vma->vm_flags & VM_MAYSHARE) {
			int err = huge_add_to_page_cache(page, mapping, idx);
			if (err) {
				put_page(page);
				if (err == -EEXIST)
					goto retry;
				goto out;
			}
		} else {
			lock_page(page);
			if (unlikely(anon_vma_prepare(vma))) {
				ret = VM_FAULT_OOM;
				goto backout_unlocked;
			}
			anon_rmap = 1;
		}
	} else {
		 
		if (unlikely(PageHWPoison(page))) {
			ret = VM_FAULT_HWPOISON |
				VM_FAULT_SET_HINDEX(hstate_index(h));
			goto backout_unlocked;
		}
	}

	 
	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		if (vma_needs_reservation(h, vma, address) < 0) {
			ret = VM_FAULT_OOM;
			goto backout_unlocked;
		}
		 
		vma_end_reservation(h, vma, address);
	}

	ptl = huge_pte_lock(h, mm, ptep);
	size = i_size_read(mapping->host) >> huge_page_shift(h);
	if (idx >= size)
		goto backout;

	ret = 0;
	if (!huge_pte_none(huge_ptep_get(ptep)))
		goto backout;

	if (anon_rmap) {
		ClearPagePrivate(page);
		hugepage_add_new_anon_rmap(page, vma, address);
	} else
		page_dup_rmap(page, true);
	new_pte = make_huge_pte(vma, page, ((vma->vm_flags & VM_WRITE)
				&& (vma->vm_flags & VM_SHARED)));
	set_huge_pte_at(mm, address, ptep, new_pte);

	hugetlb_count_add(pages_per_huge_page(h), mm);
	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		 
		ret = hugetlb_cow(mm, vma, address, ptep, page, ptl);
	}

	spin_unlock(ptl);
	unlock_page(page);
out:
	return ret;

backout:
	spin_unlock(ptl);
backout_unlocked:
	unlock_page(page);
	restore_reserve_on_error(h, vma, address, page);
	put_page(page);
	goto out;
}
