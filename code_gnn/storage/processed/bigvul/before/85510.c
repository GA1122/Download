static int ocfs2_try_to_write_inline_data(struct address_space *mapping,
					  struct inode *inode, loff_t pos,
					  unsigned len, struct page *mmap_page,
					  struct ocfs2_write_ctxt *wc)
{
	int ret, written = 0;
	loff_t end = pos + len;
	struct ocfs2_inode_info *oi = OCFS2_I(inode);
	struct ocfs2_dinode *di = NULL;

	trace_ocfs2_try_to_write_inline_data((unsigned long long)oi->ip_blkno,
					     len, (unsigned long long)pos,
					     oi->ip_dyn_features);

	 
	if (oi->ip_dyn_features & OCFS2_INLINE_DATA_FL) {
		if (mmap_page == NULL &&
		    ocfs2_size_fits_inline_data(wc->w_di_bh, end))
			goto do_inline_write;

		 
		ret = ocfs2_convert_inline_data_to_extents(inode, wc->w_di_bh);
		if (ret)
			mlog_errno(ret);
		goto out;
	}

	 
	if (oi->ip_clusters != 0 || i_size_read(inode) != 0)
		return 0;

	 
	di = (struct ocfs2_dinode *)wc->w_di_bh->b_data;
	if (mmap_page ||
	    end > ocfs2_max_inline_data_with_xattr(inode->i_sb, di))
		return 0;

do_inline_write:
	ret = ocfs2_write_begin_inline(mapping, inode, wc);
	if (ret) {
		mlog_errno(ret);
		goto out;
	}

	 
	written = 1;
out:
	return written ? written : ret;
}