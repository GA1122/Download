static int __ext4_ext_dirty(const char *where, unsigned int line,
			    handle_t *handle, struct inode *inode,
			    struct ext4_ext_path *path)
{
	int err;
	if (path->p_bh) {
		ext4_extent_block_csum_set(inode, ext_block_hdr(path->p_bh));
		 
		err = __ext4_handle_dirty_metadata(where, line, handle,
						   inode, path->p_bh);
	} else {
		 
		err = ext4_mark_inode_dirty(handle, inode);
	}
	return err;
}