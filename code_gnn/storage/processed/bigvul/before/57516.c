int ext4_getattr(struct vfsmount *mnt, struct dentry *dentry,
		 struct kstat *stat)
{
	struct inode *inode;
	unsigned long delalloc_blocks;

	inode = dentry->d_inode;
	generic_fillattr(inode, stat);

	 
	spin_lock(&EXT4_I(inode)->i_block_reservation_lock);
	delalloc_blocks = EXT4_I(inode)->i_reserved_data_blocks;
	spin_unlock(&EXT4_I(inode)->i_block_reservation_lock);

	stat->blocks += (delalloc_blocks << inode->i_sb->s_blocksize_bits)>>9;
	return 0;
}
