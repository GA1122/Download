int filemap_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	int error;
	struct file *file = vma->vm_file;
	struct address_space *mapping = file->f_mapping;
	struct file_ra_state *ra = &file->f_ra;
	struct inode *inode = mapping->host;
	struct page *page;
	unsigned long size;
	int did_readaround = 0;
	int ret = 0;

	size = (i_size_read(inode) + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	if (vmf->pgoff >= size)
		return VM_FAULT_SIGBUS;

	 
	if (VM_RandomReadHint(vma))
		goto no_cached_page;

	 
retry_find:
	page = find_lock_page(mapping, vmf->pgoff);
	 
	if (VM_SequentialReadHint(vma)) {
		if (!page) {
			page_cache_sync_readahead(mapping, ra, file,
							   vmf->pgoff, 1);
			page = find_lock_page(mapping, vmf->pgoff);
			if (!page)
				goto no_cached_page;
		}
		if (PageReadahead(page)) {
			page_cache_async_readahead(mapping, ra, file, page,
							   vmf->pgoff, 1);
		}
	}

	if (!page) {
		unsigned long ra_pages;

		ra->mmap_miss++;

		 
		if (ra->mmap_miss > MMAP_LOTSAMISS)
			goto no_cached_page;

		 
		if (!did_readaround) {
			ret = VM_FAULT_MAJOR;
			count_vm_event(PGMAJFAULT);
		}
		did_readaround = 1;
		ra_pages = max_sane_readahead(file->f_ra.ra_pages);
		if (ra_pages) {
			pgoff_t start = 0;

			if (vmf->pgoff > ra_pages / 2)
				start = vmf->pgoff - ra_pages / 2;
			do_page_cache_readahead(mapping, file, start, ra_pages);
		}
		page = find_lock_page(mapping, vmf->pgoff);
		if (!page)
			goto no_cached_page;
	}

	if (!did_readaround)
		ra->mmap_miss--;

	 
	if (unlikely(!PageUptodate(page)))
		goto page_not_uptodate;

	 
	size = (i_size_read(inode) + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	if (unlikely(vmf->pgoff >= size)) {
		unlock_page(page);
		page_cache_release(page);
		return VM_FAULT_SIGBUS;
	}

	 
	mark_page_accessed(page);
	ra->prev_pos = (loff_t)page->index << PAGE_CACHE_SHIFT;
	vmf->page = page;
	return ret | VM_FAULT_LOCKED;

no_cached_page:
	 
	error = page_cache_read(file, vmf->pgoff);

	 
	if (error >= 0)
		goto retry_find;

	 
	if (error == -ENOMEM)
		return VM_FAULT_OOM;
	return VM_FAULT_SIGBUS;

page_not_uptodate:
	 
	if (!did_readaround) {
		ret = VM_FAULT_MAJOR;
		count_vm_event(PGMAJFAULT);
	}

	 
	ClearPageError(page);
	error = mapping->a_ops->readpage(file, page);
	page_cache_release(page);

	if (!error || error == AOP_TRUNCATED_PAGE)
		goto retry_find;

	 
	shrink_readahead_size_eio(file, ra);
	return VM_FAULT_SIGBUS;
}
