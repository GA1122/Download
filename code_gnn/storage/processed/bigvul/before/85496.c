 int ocfs2_get_block(struct inode *inode, sector_t iblock,
 		    struct buffer_head *bh_result, int create)
 {
	int err = 0;
	unsigned int ext_flags;
	u64 max_blocks = bh_result->b_size >> inode->i_blkbits;
	u64 p_blkno, count, past_eof;
	struct ocfs2_super *osb = OCFS2_SB(inode->i_sb);

	trace_ocfs2_get_block((unsigned long long)OCFS2_I(inode)->ip_blkno,
			      (unsigned long long)iblock, bh_result, create);

	if (OCFS2_I(inode)->ip_flags & OCFS2_INODE_SYSTEM_FILE)
		mlog(ML_NOTICE, "get_block on system inode 0x%p (%lu)\n",
		     inode, inode->i_ino);

	if (S_ISLNK(inode->i_mode)) {
		 
		err = ocfs2_symlink_get_block(inode, iblock, bh_result, create);
		goto bail;
	}

	err = ocfs2_extent_map_get_blocks(inode, iblock, &p_blkno, &count,
					  &ext_flags);
	if (err) {
		mlog(ML_ERROR, "Error %d from get_blocks(0x%p, %llu, 1, "
		     "%llu, NULL)\n", err, inode, (unsigned long long)iblock,
		     (unsigned long long)p_blkno);
		goto bail;
	}

	if (max_blocks < count)
		count = max_blocks;

	 
	if (create && p_blkno == 0 && ocfs2_sparse_alloc(osb)) {
		clear_buffer_dirty(bh_result);
		clear_buffer_uptodate(bh_result);
		goto bail;
	}

	 
	if (p_blkno && !(ext_flags & OCFS2_EXT_UNWRITTEN))
		map_bh(bh_result, inode->i_sb, p_blkno);

	bh_result->b_size = count << inode->i_blkbits;

	if (!ocfs2_sparse_alloc(osb)) {
		if (p_blkno == 0) {
			err = -EIO;
			mlog(ML_ERROR,
			     "iblock = %llu p_blkno = %llu blkno=(%llu)\n",
			     (unsigned long long)iblock,
			     (unsigned long long)p_blkno,
			     (unsigned long long)OCFS2_I(inode)->ip_blkno);
			mlog(ML_ERROR, "Size %llu, clusters %u\n", (unsigned long long)i_size_read(inode), OCFS2_I(inode)->ip_clusters);
			dump_stack();
			goto bail;
		}
	}

	past_eof = ocfs2_blocks_for_bytes(inode->i_sb, i_size_read(inode));

	trace_ocfs2_get_block_end((unsigned long long)OCFS2_I(inode)->ip_blkno,
				  (unsigned long long)past_eof);
	if (create && (iblock >= past_eof))
		set_buffer_new(bh_result);

bail:
	if (err < 0)
		err = -EIO;

	return err;
}