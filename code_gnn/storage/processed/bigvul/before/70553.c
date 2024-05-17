static void lockdep_set_quota_inode(struct inode *inode, int subclass)
{
	struct ext4_inode_info *ei = EXT4_I(inode);

	 
	(void) ei;	 
	lockdep_set_subclass(&ei->i_data_sem, subclass);
}
