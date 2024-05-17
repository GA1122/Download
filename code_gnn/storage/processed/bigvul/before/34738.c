 static int ext4_convert_unwritten_extents_endio(handle_t *handle,
					      struct inode *inode,
					      struct ext4_ext_path *path)
{
	struct ext4_extent *ex;
	struct ext4_extent_header *eh;
	int depth;
	int err = 0;

	depth = ext_depth(inode);
	eh = path[depth].p_hdr;
	ex = path[depth].p_ext;

	ext_debug("ext4_convert_unwritten_extents_endio: inode %lu, logical"
		"block %llu, max_blocks %u\n", inode->i_ino,
		(unsigned long long)le32_to_cpu(ex->ee_block),
		ext4_ext_get_actual_len(ex));

	err = ext4_ext_get_access(handle, inode, path + depth);
	if (err)
		goto out;
	 
	ext4_ext_mark_initialized(ex);

	 
	ext4_ext_try_to_merge(inode, path, ex);

	 
	err = ext4_ext_dirty(handle, inode, path + depth);
out:
	ext4_ext_show_leaf(inode, path);
	return err;
}