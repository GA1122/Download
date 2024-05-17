xfs_writepage_map(
	struct xfs_writepage_ctx *wpc,
	struct writeback_control *wbc,
	struct inode		*inode,
	struct page		*page,
	loff_t			offset,
	__uint64_t              end_offset)
{
	LIST_HEAD(submit_list);
	struct xfs_ioend	*ioend, *next;
	struct buffer_head	*bh, *head;
	ssize_t			len = 1 << inode->i_blkbits;
	int			error = 0;
	int			count = 0;
	int			uptodate = 1;
	unsigned int		new_type;

	bh = head = page_buffers(page);
	offset = page_offset(page);
	do {
		if (offset >= end_offset)
			break;
		if (!buffer_uptodate(bh))
			uptodate = 0;

		 
		if (!buffer_mapped(bh) && buffer_uptodate(bh)) {
			wpc->imap_valid = false;
			continue;
		}

		if (buffer_unwritten(bh))
			new_type = XFS_IO_UNWRITTEN;
		else if (buffer_delay(bh))
			new_type = XFS_IO_DELALLOC;
		else if (buffer_uptodate(bh))
			new_type = XFS_IO_OVERWRITE;
		else {
			if (PageUptodate(page))
				ASSERT(buffer_mapped(bh));
			 
			wpc->imap_valid = false;
			continue;
		}

		if (xfs_is_reflink_inode(XFS_I(inode))) {
			error = xfs_map_cow(wpc, inode, offset, &new_type);
			if (error)
				goto out;
		}

		if (wpc->io_type != new_type) {
			wpc->io_type = new_type;
			wpc->imap_valid = false;
		}

		if (wpc->imap_valid)
			wpc->imap_valid = xfs_imap_valid(inode, &wpc->imap,
							 offset);
		if (!wpc->imap_valid) {
			error = xfs_map_blocks(inode, offset, &wpc->imap,
					     wpc->io_type);
			if (error)
				goto out;
			wpc->imap_valid = xfs_imap_valid(inode, &wpc->imap,
							 offset);
		}
		if (wpc->imap_valid) {
			lock_buffer(bh);
			if (wpc->io_type != XFS_IO_OVERWRITE)
				xfs_map_at_offset(inode, bh, &wpc->imap, offset);
			xfs_add_to_ioend(inode, bh, offset, wpc, wbc, &submit_list);
			count++;
		}

	} while (offset += len, ((bh = bh->b_this_page) != head));

	if (uptodate && bh == head)
		SetPageUptodate(page);

	ASSERT(wpc->ioend || list_empty(&submit_list));

out:
	 
	if (count) {
		xfs_start_page_writeback(page, !error);

		 
		list_for_each_entry_safe(ioend, next, &submit_list, io_list) {
			int error2;

			list_del_init(&ioend->io_list);
			error2 = xfs_submit_ioend(wbc, ioend, error);
			if (error2 && !error)
				error = error2;
		}
	} else if (error) {
		xfs_aops_discard_page(page);
		ClearPageUptodate(page);
		unlock_page(page);
	} else {
		 
		xfs_start_page_writeback(page, 1);
		end_page_writeback(page);
	}

	mapping_set_error(page->mapping, error);
	return error;
}
