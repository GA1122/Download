static int gfs2_block_truncate_page(struct address_space *mapping, loff_t from)
{
	struct inode *inode = mapping->host;
	struct gfs2_inode *ip = GFS2_I(inode);
	unsigned long index = from >> PAGE_CACHE_SHIFT;
	unsigned offset = from & (PAGE_CACHE_SIZE-1);
	unsigned blocksize, iblock, length, pos;
	struct buffer_head *bh;
	struct page *page;
	int err;

	page = grab_cache_page(mapping, index);
	if (!page)
		return 0;

	blocksize = inode->i_sb->s_blocksize;
	length = blocksize - (offset & (blocksize - 1));
	iblock = index << (PAGE_CACHE_SHIFT - inode->i_sb->s_blocksize_bits);

	if (!page_has_buffers(page))
		create_empty_buffers(page, blocksize, 0);

	 
	bh = page_buffers(page);
	pos = blocksize;
	while (offset >= pos) {
		bh = bh->b_this_page;
		iblock++;
		pos += blocksize;
	}

	err = 0;

	if (!buffer_mapped(bh)) {
		gfs2_block_map(inode, iblock, bh, 0);
		 
		if (!buffer_mapped(bh))
			goto unlock;
	}

	 
	if (PageUptodate(page))
		set_buffer_uptodate(bh);

	if (!buffer_uptodate(bh)) {
		err = -EIO;
		ll_rw_block(READ, 1, &bh);
		wait_on_buffer(bh);
		 
		if (!buffer_uptodate(bh))
			goto unlock;
		err = 0;
	}

	if (!gfs2_is_writeback(ip))
		gfs2_trans_add_bh(ip->i_gl, bh, 0);

	zero_user(page, offset, length);
	mark_buffer_dirty(bh);
unlock:
	unlock_page(page);
	page_cache_release(page);
	return err;
}