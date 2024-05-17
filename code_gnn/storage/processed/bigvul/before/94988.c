static inline int ext4_handle_dirty_xattr_block(handle_t *handle,
						struct inode *inode,
						struct buffer_head *bh)
{
	ext4_xattr_block_csum_set(inode, bh->b_blocknr, BHDR(bh));
	return ext4_handle_dirty_metadata(handle, inode, bh);
}
