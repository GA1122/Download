  static int ext4_convert_unwritten_extents_endio(handle_t *handle,
					      struct inode *inode,
					      struct ext4_ext_path *path)
// 						struct inode *inode,
// 						struct ext4_map_blocks *map,
// 						struct ext4_ext_path *path)
  {
  	struct ext4_extent *ex;
// 	ext4_lblk_t ee_block;
// 	unsigned int ee_len;
  	int depth;
  	int err = 0;
  
  	depth = ext_depth(inode);
  	ex = path[depth].p_ext;
// 	ee_block = le32_to_cpu(ex->ee_block);
// 	ee_len = ext4_ext_get_actual_len(ex);
  
  	ext_debug("ext4_convert_unwritten_extents_endio: inode %lu, logical"
  		"block %llu, max_blocks %u\n", inode->i_ino,
		(unsigned long long)le32_to_cpu(ex->ee_block),
		ext4_ext_get_actual_len(ex));
// 		  (unsigned long long)ee_block, ee_len);
// 
// 	 
// 	if (ee_block != map->m_lblk || ee_len > map->m_len) {
// 		err = ext4_split_unwritten_extents(handle, inode, map, path,
// 						   EXT4_GET_BLOCKS_CONVERT);
// 		if (err < 0)
// 			goto out;
// 		ext4_ext_drop_refs(path);
// 		path = ext4_ext_find_extent(inode, map->m_lblk, path);
// 		if (IS_ERR(path)) {
// 			err = PTR_ERR(path);
// 			goto out;
// 		}
// 		depth = ext_depth(inode);
// 		ex = path[depth].p_ext;
// 	}
  
  	err = ext4_ext_get_access(handle, inode, path + depth);
  	if (err)
 		goto out;
 	 
 	ext4_ext_mark_initialized(ex);
 
 	 
 	ext4_ext_try_to_merge(handle, inode, path, ex);
 
 	 
 	err = ext4_ext_dirty(handle, inode, path + path->p_depth);
 out:
 	ext4_ext_show_leaf(inode, path);
 	return err;
 }