int ocfs2_write_end_nolock(struct address_space *mapping,
			   loff_t pos, unsigned len, unsigned copied, void *fsdata)
{
	int i, ret;
	unsigned from, to, start = pos & (PAGE_SIZE - 1);
	struct inode *inode = mapping->host;
	struct ocfs2_super *osb = OCFS2_SB(inode->i_sb);
	struct ocfs2_write_ctxt *wc = fsdata;
	struct ocfs2_dinode *di = (struct ocfs2_dinode *)wc->w_di_bh->b_data;
	handle_t *handle = wc->w_handle;
	struct page *tmppage;

	BUG_ON(!list_empty(&wc->w_unwritten_list));

	if (handle) {
		ret = ocfs2_journal_access_di(handle, INODE_CACHE(inode),
				wc->w_di_bh, OCFS2_JOURNAL_ACCESS_WRITE);
		if (ret) {
			copied = ret;
			mlog_errno(ret);
			goto out;
		}
	}

	if (OCFS2_I(inode)->ip_dyn_features & OCFS2_INLINE_DATA_FL) {
		ocfs2_write_end_inline(inode, pos, len, &copied, di, wc);
		goto out_write_size;
	}

	if (unlikely(copied < len) && wc->w_target_page) {
		if (!PageUptodate(wc->w_target_page))
			copied = 0;

		ocfs2_zero_new_buffers(wc->w_target_page, start+copied,
				       start+len);
	}
	if (wc->w_target_page)
		flush_dcache_page(wc->w_target_page);

	for(i = 0; i < wc->w_num_pages; i++) {
		tmppage = wc->w_pages[i];

		 
		if (tmppage == NULL)
			continue;

		if (tmppage == wc->w_target_page) {
			from = wc->w_target_from;
			to = wc->w_target_to;

			BUG_ON(from > PAGE_SIZE ||
			       to > PAGE_SIZE ||
			       to < from);
		} else {
			 
			from = 0;
			to = PAGE_SIZE;
		}

		if (page_has_buffers(tmppage)) {
			if (handle && ocfs2_should_order_data(inode))
				ocfs2_jbd2_file_inode(handle, inode);
			block_commit_write(tmppage, from, to);
		}
	}

out_write_size:
	 
	if (wc->w_type != OCFS2_WRITE_DIRECT) {
		pos += copied;
		if (pos > i_size_read(inode)) {
			i_size_write(inode, pos);
			mark_inode_dirty(inode);
		}
		inode->i_blocks = ocfs2_inode_sector_count(inode);
		di->i_size = cpu_to_le64((u64)i_size_read(inode));
		inode->i_mtime = inode->i_ctime = current_time(inode);
		di->i_mtime = di->i_ctime = cpu_to_le64(inode->i_mtime.tv_sec);
		di->i_mtime_nsec = di->i_ctime_nsec = cpu_to_le32(inode->i_mtime.tv_nsec);
		ocfs2_update_inode_fsync_trans(handle, inode, 1);
	}
	if (handle)
		ocfs2_journal_dirty(handle, wc->w_di_bh);

out:
	 
	ocfs2_unlock_pages(wc);

	if (handle)
		ocfs2_commit_trans(osb, handle);

	ocfs2_run_deallocs(osb, &wc->w_dealloc);

	brelse(wc->w_di_bh);
	kfree(wc);

	return copied;
}