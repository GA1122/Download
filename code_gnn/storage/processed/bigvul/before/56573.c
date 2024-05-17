int ext4_get_inode_loc(struct inode *inode, struct ext4_iloc *iloc)
{
	 
	return __ext4_get_inode_loc(inode, iloc,
		!ext4_test_inode_state(inode, EXT4_STATE_XATTR));
}
