int ext4_mark_iloc_dirty(handle_t *handle,
			 struct inode *inode, struct ext4_iloc *iloc)
{
	int err = 0;

	if (IS_I_VERSION(inode))
		inode_inc_iversion(inode);

	 
	get_bh(iloc->bh);

	 
	err = ext4_do_update_inode(handle, inode, iloc);
	put_bh(iloc->bh);
	return err;
}
