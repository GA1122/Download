static int hugetlb_no_page(struct mm_struct *mm, struct vm_area_struct *vma,
			unsigned long address, pte_t *ptep, unsigned int flags)
{
	struct hstate *h = hstate_vma(vma);
	int ret = VM_FAULT_SIGBUS;
	int anon_rmap = 0;
	pgoff_t idx;
	unsigned long size;
	struct page *page;
	struct address_space *mapping;
	pte_t new_pte;

	 
	if (is_vma_resv_set(vma, HPAGE_RESV_UNMAPPED)) {
		printk(KERN_WARNING
			"PID %d killed due to inadequate hugepage pool\n",
			current->pid);
		return ret;
	}

	mapping = vma->vm_file->f_mapping;
	idx = vma_hugecache_offset(h, vma, address);

	 
retry:
	page = find_lock_page(mapping, idx);
	if (!page) {
		size = i_size_read(mapping->host) >> huge_page_shift(h);
		if (idx >= size)
			goto out;
		page = alloc_huge_page(vma, address, 0);
		if (IS_ERR(page)) {
			ret = -PTR_ERR(page);
			goto out;
		}
		clear_huge_page(page, address, pages_per_huge_page(h));
		__SetPageUptodate(page);

		if (vma->vm_flags & VM_MAYSHARE) {
			int err;
			struct inode *inode = mapping->host;

			err = add_to_page_cache(page, mapping, idx, GFP_KERNEL);
			if (err) {
				put_page(page);
				if (err == -EEXIST)
					goto retry;
				goto out;
			}

			spin_lock(&inode->i_lock);
			inode->i_blocks += blocks_per_huge_page(h);
			spin_unlock(&inode->i_lock);
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
			      VM_FAULT_SET_HINDEX(h - hstates);
			goto backout_unlocked;
		}
	}

	 
	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED))
		if (vma_needs_reservation(h, vma, address) < 0) {
			ret = VM_FAULT_OOM;
			goto backout_unlocked;
		}

	spin_lock(&mm->page_table_lock);
	size = i_size_read(mapping->host) >> huge_page_shift(h);
	if (idx >= size)
		goto backout;

	ret = 0;
	if (!huge_pte_none(huge_ptep_get(ptep)))
		goto backout;

	if (anon_rmap)
		hugepage_add_new_anon_rmap(page, vma, address);
	else
		page_dup_rmap(page);
	new_pte = make_huge_pte(vma, page, ((vma->vm_flags & VM_WRITE)
				&& (vma->vm_flags & VM_SHARED)));
	set_huge_pte_at(mm, address, ptep, new_pte);

	if ((flags & FAULT_FLAG_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		 
		ret = hugetlb_cow(mm, vma, address, ptep, new_pte, page);
	}

	spin_unlock(&mm->page_table_lock);
	unlock_page(page);
out:
	return ret;

backout:
	spin_unlock(&mm->page_table_lock);
backout_unlocked:
	unlock_page(page);
	put_page(page);
	goto out;
}