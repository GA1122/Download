static int ext4_journalled_write_end(struct file *file,
				     struct address_space *mapping,
				     loff_t pos, unsigned len, unsigned copied,
				     struct page *page, void *fsdata)
{
	handle_t *handle = ext4_journal_current_handle();
	struct inode *inode = mapping->host;
	loff_t old_size = inode->i_size;
	int ret = 0, ret2;
	int partial = 0;
	unsigned from, to;
	int size_changed = 0;

	trace_ext4_journalled_write_end(inode, pos, len, copied);
	from = pos & (PAGE_SIZE - 1);
	to = from + len;

	BUG_ON(!ext4_handle_valid(handle));

	if (ext4_has_inline_data(inode))
		copied = ext4_write_inline_data_end(inode, pos, len,
						    copied, page);
	else {
		if (copied < len) {
			if (!PageUptodate(page))
				copied = 0;
			zero_new_buffers(page, from+copied, to);
		}

		ret = ext4_walk_page_buffers(handle, page_buffers(page), from,
					     to, &partial, write_end_fn);
		if (!partial)
			SetPageUptodate(page);
	}
	size_changed = ext4_update_inode_size(inode, pos + copied);
	ext4_set_inode_state(inode, EXT4_STATE_JDATA);
	EXT4_I(inode)->i_datasync_tid = handle->h_transaction->t_tid;
	unlock_page(page);
	put_page(page);

	if (old_size < pos)
		pagecache_isize_extended(inode, old_size, pos);

	if (size_changed) {
		ret2 = ext4_mark_inode_dirty(handle, inode);
		if (!ret)
			ret = ret2;
	}

	if (pos + len > inode->i_size && ext4_can_truncate(inode))
		 
		ext4_orphan_add(handle, inode);

	ret2 = ext4_journal_stop(handle);
	if (!ret)
		ret = ret2;
	if (pos + len > inode->i_size) {
		ext4_truncate_failed_write(inode);
		 
		if (inode->i_nlink)
			ext4_orphan_del(NULL, inode);
	}

	return ret ? ret : copied;
}