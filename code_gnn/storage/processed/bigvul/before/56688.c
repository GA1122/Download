static ssize_t ext4_quota_write(struct super_block *sb, int type,
				const char *data, size_t len, loff_t off)
{
	struct inode *inode = sb_dqopt(sb)->files[type];
	ext4_lblk_t blk = off >> EXT4_BLOCK_SIZE_BITS(sb);
	int err, offset = off & (sb->s_blocksize - 1);
	int retries = 0;
	struct buffer_head *bh;
	handle_t *handle = journal_current_handle();

	if (EXT4_SB(sb)->s_journal && !handle) {
		ext4_msg(sb, KERN_WARNING, "Quota write (off=%llu, len=%llu)"
			" cancelled because transaction is not started",
			(unsigned long long)off, (unsigned long long)len);
		return -EIO;
	}
	 
	if (sb->s_blocksize - offset < len) {
		ext4_msg(sb, KERN_WARNING, "Quota write (off=%llu, len=%llu)"
			" cancelled because not block aligned",
			(unsigned long long)off, (unsigned long long)len);
		return -EIO;
	}

	do {
		bh = ext4_bread(handle, inode, blk,
				EXT4_GET_BLOCKS_CREATE |
				EXT4_GET_BLOCKS_METADATA_NOFAIL);
	} while (IS_ERR(bh) && (PTR_ERR(bh) == -ENOSPC) &&
		 ext4_should_retry_alloc(inode->i_sb, &retries));
	if (IS_ERR(bh))
		return PTR_ERR(bh);
	if (!bh)
		goto out;
	BUFFER_TRACE(bh, "get write access");
	err = ext4_journal_get_write_access(handle, bh);
	if (err) {
		brelse(bh);
		return err;
	}
	lock_buffer(bh);
	memcpy(bh->b_data+offset, data, len);
	flush_dcache_page(bh->b_page);
	unlock_buffer(bh);
	err = ext4_handle_dirty_metadata(handle, NULL, bh);
	brelse(bh);
out:
	if (inode->i_size < off + len) {
		i_size_write(inode, off + len);
		EXT4_I(inode)->i_disksize = inode->i_size;
		ext4_mark_inode_dirty(handle, inode);
	}
	return len;
}