ext4_ext_shift_extents(struct inode *inode, handle_t *handle,
		       ext4_lblk_t start, ext4_lblk_t shift,
		       enum SHIFT_DIRECTION SHIFT)
{
	struct ext4_ext_path *path;
	int ret = 0, depth;
	struct ext4_extent *extent;
	ext4_lblk_t stop, *iterator, ex_start, ex_end;

	 
	path = ext4_find_extent(inode, EXT_MAX_BLOCKS - 1, NULL, 0);
	if (IS_ERR(path))
		return PTR_ERR(path);

	depth = path->p_depth;
	extent = path[depth].p_ext;
	if (!extent)
		goto out;

	stop = le32_to_cpu(extent->ee_block) +
			ext4_ext_get_actual_len(extent);

        
	if (SHIFT == SHIFT_LEFT) {
		path = ext4_find_extent(inode, start - 1, &path, 0);
		if (IS_ERR(path))
			return PTR_ERR(path);
		depth = path->p_depth;
		extent =  path[depth].p_ext;
		if (extent) {
			ex_start = le32_to_cpu(extent->ee_block);
			ex_end = le32_to_cpu(extent->ee_block) +
				ext4_ext_get_actual_len(extent);
		} else {
			ex_start = 0;
			ex_end = 0;
		}

		if ((start == ex_start && shift > ex_start) ||
		    (shift > start - ex_end)) {
			ext4_ext_drop_refs(path);
			kfree(path);
			return -EINVAL;
		}
	}

	 
	if (SHIFT == SHIFT_LEFT)
		iterator = &start;
	else
		iterator = &stop;

	 
	while (start < stop) {
		path = ext4_find_extent(inode, *iterator, &path, 0);
		if (IS_ERR(path))
			return PTR_ERR(path);
		depth = path->p_depth;
		extent = path[depth].p_ext;
		if (!extent) {
			EXT4_ERROR_INODE(inode, "unexpected hole at %lu",
					 (unsigned long) *iterator);
			return -EFSCORRUPTED;
		}
		if (SHIFT == SHIFT_LEFT && *iterator >
		    le32_to_cpu(extent->ee_block)) {
			 
			if (extent < EXT_LAST_EXTENT(path[depth].p_hdr)) {
				path[depth].p_ext++;
			} else {
				*iterator = ext4_ext_next_allocated_block(path);
				continue;
			}
		}

		if (SHIFT == SHIFT_LEFT) {
			extent = EXT_LAST_EXTENT(path[depth].p_hdr);
			*iterator = le32_to_cpu(extent->ee_block) +
					ext4_ext_get_actual_len(extent);
		} else {
			extent = EXT_FIRST_EXTENT(path[depth].p_hdr);
			*iterator =  le32_to_cpu(extent->ee_block) > 0 ?
				le32_to_cpu(extent->ee_block) - 1 : 0;
			 
			while (le32_to_cpu(extent->ee_block) < start)
				extent++;
			path[depth].p_ext = extent;
		}
		ret = ext4_ext_shift_path_extents(path, shift, inode,
				handle, SHIFT);
		if (ret)
			break;
	}
out:
	ext4_ext_drop_refs(path);
	kfree(path);
	return ret;
}