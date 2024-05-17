static ssize_t ext3_quota_write(struct super_block *sb, int type,
				const char *data, size_t len, loff_t off)
{
	struct inode *inode = sb_dqopt(sb)->files[type];
	sector_t blk = off >> EXT3_BLOCK_SIZE_BITS(sb);
	int err = 0;
	int offset = off & (sb->s_blocksize - 1);
	int journal_quota = EXT3_SB(sb)->s_qf_names[type] != NULL;
	struct buffer_head *bh;
	handle_t *handle = journal_current_handle();

	if (!handle) {
		ext3_msg(sb, KERN_WARNING,
			"warning: quota write (off=%llu, len=%llu)"
			" cancelled because transaction is not started.",
			(unsigned long long)off, (unsigned long long)len);
		return -EIO;
	}

	 
	if (sb->s_blocksize - offset < len) {
		ext3_msg(sb, KERN_WARNING, "Quota write (off=%llu, len=%llu)"
			" cancelled because not block aligned",
			(unsigned long long)off, (unsigned long long)len);
		return -EIO;
	}
	bh = ext3_bread(handle, inode, blk, 1, &err);
	if (!bh)
		goto out;
	if (journal_quota) {
		err = ext3_journal_get_write_access(handle, bh);
		if (err) {
			brelse(bh);
			goto out;
		}
	}
	lock_buffer(bh);
	memcpy(bh->b_data+offset, data, len);
	flush_dcache_page(bh->b_page);
	unlock_buffer(bh);
	if (journal_quota)
		err = ext3_journal_dirty_metadata(handle, bh);
	else {
		 
		err = ext3_journal_dirty_data(handle, bh);
		mark_buffer_dirty(bh);
	}
	brelse(bh);
out:
	if (err)
		return err;
	if (inode->i_size < off + len) {
		i_size_write(inode, off + len);
		EXT3_I(inode)->i_disksize = inode->i_size;
	}
	inode->i_version++;
	inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	ext3_mark_inode_dirty(handle, inode);
	return len;
}