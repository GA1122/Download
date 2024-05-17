ext4_xattr_check_block(struct inode *inode, struct buffer_head *bh)
{
	int error;

	if (buffer_verified(bh))
		return 0;

	if (BHDR(bh)->h_magic != cpu_to_le32(EXT4_XATTR_MAGIC) ||
	    BHDR(bh)->h_blocks != cpu_to_le32(1))
		return -EFSCORRUPTED;
	if (!ext4_xattr_block_csum_verify(inode, bh->b_blocknr, BHDR(bh)))
		return -EFSBADCRC;
	error = ext4_xattr_check_names(BFIRST(bh), bh->b_data + bh->b_size,
				       bh->b_data);
	if (!error)
		set_buffer_verified(bh);
	return error;
}