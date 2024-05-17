static int ext4_convert_unwritten_extents_endio(handle_t *handle,
					      struct inode *inode,
					      struct ext4_ext_path *path)
{
	struct ext4_extent *ex;
	struct ext4_extent_header *eh;
	int depth;
	int err = 0;
	int ret = 0;

	depth = ext_depth(inode);
	eh = path[depth].p_hdr;
	ex = path[depth].p_ext;

	err = ext4_ext_get_access(handle, inode, path + depth);
	if (err)
		goto out;
	 
	ext4_ext_mark_initialized(ex);

	 
	if (ex > EXT_FIRST_EXTENT(eh)) {
		 
		ret = ext4_ext_try_to_merge(inode, path, ex - 1);
		if (ret) {
			err = ext4_ext_correct_indexes(handle, inode, path);
			if (err)
				goto out;
			depth = ext_depth(inode);
			ex--;
		}
	}
	 
	ret = ext4_ext_try_to_merge(inode, path, ex);
	if (ret) {
		err = ext4_ext_correct_indexes(handle, inode, path);
		if (err)
			goto out;
		depth = ext_depth(inode);
	}
	 
	err = ext4_ext_dirty(handle, inode, path + depth);
out:
	ext4_ext_show_leaf(inode, path);
	return err;
}
