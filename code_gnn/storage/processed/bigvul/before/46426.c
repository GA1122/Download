static int ubifs_write_begin(struct file *file, struct address_space *mapping,
			     loff_t pos, unsigned len, unsigned flags,
			     struct page **pagep, void **fsdata)
{
	struct inode *inode = mapping->host;
	struct ubifs_info *c = inode->i_sb->s_fs_info;
	struct ubifs_inode *ui = ubifs_inode(inode);
	pgoff_t index = pos >> PAGE_CACHE_SHIFT;
	int uninitialized_var(err), appending = !!(pos + len > inode->i_size);
	int skipped_read = 0;
	struct page *page;

	ubifs_assert(ubifs_inode(inode)->ui_size == inode->i_size);
	ubifs_assert(!c->ro_media && !c->ro_mount);

	if (unlikely(c->ro_error))
		return -EROFS;

	 
	page = grab_cache_page_write_begin(mapping, index, flags);
	if (unlikely(!page))
		return -ENOMEM;

	if (!PageUptodate(page)) {
		 
		if (!(pos & ~PAGE_CACHE_MASK) && len == PAGE_CACHE_SIZE) {
			 
			SetPageChecked(page);
			skipped_read = 1;
		} else {
			err = do_readpage(page);
			if (err) {
				unlock_page(page);
				page_cache_release(page);
				return err;
			}
		}

		SetPageUptodate(page);
		ClearPageError(page);
	}

	err = allocate_budget(c, page, ui, appending);
	if (unlikely(err)) {
		ubifs_assert(err == -ENOSPC);
		 
		if (skipped_read) {
			ClearPageChecked(page);
			ClearPageUptodate(page);
		}
		 
		if (appending) {
			ubifs_assert(mutex_is_locked(&ui->ui_mutex));
			mutex_unlock(&ui->ui_mutex);
		}
		unlock_page(page);
		page_cache_release(page);

		return write_begin_slow(mapping, pos, len, pagep, flags);
	}

	 
	*pagep = page;
	return 0;

}
