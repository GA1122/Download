static int ext4_ext_dirty(handle_t *handle, struct inode *inode,
				struct ext4_ext_path *path)
{
	int err;
	if (path->p_bh) {
		 
		err = ext4_handle_dirty_metadata(handle, inode, path->p_bh);
	} else {
		 
		err = ext4_mark_inode_dirty(handle, inode);
	}
	return err;
}
