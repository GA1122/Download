static int ext4_ext_get_access(handle_t *handle, struct inode *inode,
				struct ext4_ext_path *path)
{
	if (path->p_bh) {
		 
		return ext4_journal_get_write_access(handle, path->p_bh);
	}
	 
	 
	return 0;
}