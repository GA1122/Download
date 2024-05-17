static int ext4_set_context(struct inode *inode, const void *ctx, size_t len,
							void *fs_data)
{
	handle_t *handle = fs_data;
	int res, res2, retries = 0;

	 

	if (handle) {
		res = ext4_xattr_set_handle(handle, inode,
					    EXT4_XATTR_INDEX_ENCRYPTION,
					    EXT4_XATTR_NAME_ENCRYPTION_CONTEXT,
					    ctx, len, 0);
		if (!res) {
			ext4_set_inode_flag(inode, EXT4_INODE_ENCRYPT);
			ext4_clear_inode_state(inode,
					EXT4_STATE_MAY_INLINE_DATA);
			 
			ext4_set_inode_flags(inode);
		}
		return res;
	}

retry:
	handle = ext4_journal_start(inode, EXT4_HT_MISC,
			ext4_jbd2_credits_xattr(inode));
	if (IS_ERR(handle))
		return PTR_ERR(handle);

	res = ext4_xattr_set_handle(handle, inode, EXT4_XATTR_INDEX_ENCRYPTION,
				    EXT4_XATTR_NAME_ENCRYPTION_CONTEXT,
				    ctx, len, 0);
	if (!res) {
		ext4_set_inode_flag(inode, EXT4_INODE_ENCRYPT);
		 
		ext4_set_inode_flags(inode);
		res = ext4_mark_inode_dirty(handle, inode);
		if (res)
			EXT4_ERROR_INODE(inode, "Failed to mark inode dirty");
	}
	res2 = ext4_journal_stop(handle);

	if (res == -ENOSPC && ext4_should_retry_alloc(inode->i_sb, &retries))
		goto retry;
	if (!res)
		res = res2;
	return res;
}
