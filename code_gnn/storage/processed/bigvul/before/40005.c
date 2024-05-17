static int cifs_readpages(struct file *file, struct address_space *mapping,
	struct list_head *page_list, unsigned num_pages)
{
	int rc;
	struct list_head tmplist;
	struct cifsFileInfo *open_file = file->private_data;
	struct cifs_sb_info *cifs_sb = CIFS_SB(file->f_path.dentry->d_sb);
	unsigned int rsize = cifs_sb->rsize;
	pid_t pid;

	 
	if (unlikely(rsize < PAGE_CACHE_SIZE))
		return 0;

	 
	rc = cifs_readpages_from_fscache(mapping->host, mapping, page_list,
					 &num_pages);
	if (rc == 0)
		return rc;

	if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_RWPIDFORWARD)
		pid = open_file->pid;
	else
		pid = current->tgid;

	rc = 0;
	INIT_LIST_HEAD(&tmplist);

	cifs_dbg(FYI, "%s: file=%p mapping=%p num_pages=%u\n",
		 __func__, file, mapping, num_pages);

	 
	while (!list_empty(page_list)) {
		unsigned int i;
		unsigned int bytes = PAGE_CACHE_SIZE;
		unsigned int expected_index;
		unsigned int nr_pages = 1;
		loff_t offset;
		struct page *page, *tpage;
		struct cifs_readdata *rdata;

		page = list_entry(page_list->prev, struct page, lru);

		 
		__set_page_locked(page);
		rc = add_to_page_cache_locked(page, mapping,
					      page->index, GFP_KERNEL);

		 
		if (rc) {
			__clear_page_locked(page);
			break;
		}

		 
		offset = (loff_t)page->index << PAGE_CACHE_SHIFT;
		list_move_tail(&page->lru, &tmplist);

		 
		expected_index = page->index + 1;
		list_for_each_entry_safe_reverse(page, tpage, page_list, lru) {
			 
			if (page->index != expected_index)
				break;

			 
			if (bytes + PAGE_CACHE_SIZE > rsize)
				break;

			__set_page_locked(page);
			if (add_to_page_cache_locked(page, mapping,
						page->index, GFP_KERNEL)) {
				__clear_page_locked(page);
				break;
			}
			list_move_tail(&page->lru, &tmplist);
			bytes += PAGE_CACHE_SIZE;
			expected_index++;
			nr_pages++;
		}

		rdata = cifs_readdata_alloc(nr_pages, cifs_readv_complete);
		if (!rdata) {
			 
			list_for_each_entry_safe(page, tpage, &tmplist, lru) {
				list_del(&page->lru);
				lru_cache_add_file(page);
				unlock_page(page);
				page_cache_release(page);
			}
			rc = -ENOMEM;
			break;
		}

		rdata->cfile = cifsFileInfo_get(open_file);
		rdata->mapping = mapping;
		rdata->offset = offset;
		rdata->bytes = bytes;
		rdata->pid = pid;
		rdata->pagesz = PAGE_CACHE_SIZE;
		rdata->read_into_pages = cifs_readpages_read_into_pages;

		list_for_each_entry_safe(page, tpage, &tmplist, lru) {
			list_del(&page->lru);
			rdata->pages[rdata->nr_pages++] = page;
		}

		rc = cifs_retry_async_readv(rdata);
		if (rc != 0) {
			for (i = 0; i < rdata->nr_pages; i++) {
				page = rdata->pages[i];
				lru_cache_add_file(page);
				unlock_page(page);
				page_cache_release(page);
			}
			kref_put(&rdata->refcount, cifs_readdata_release);
			break;
		}

		kref_put(&rdata->refcount, cifs_readdata_release);
	}

	 
	cifs_fscache_readpages_cancel(mapping->host, page_list);
	return rc;
}
