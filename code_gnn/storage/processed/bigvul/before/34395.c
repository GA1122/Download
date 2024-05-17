int btrfs_defrag_file(struct inode *inode, struct file *file,
		      struct btrfs_ioctl_defrag_range_args *range,
		      u64 newer_than, unsigned long max_to_defrag)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct file_ra_state *ra = NULL;
	unsigned long last_index;
	u64 isize = i_size_read(inode);
	u64 last_len = 0;
	u64 skip = 0;
	u64 defrag_end = 0;
	u64 newer_off = range->start;
	unsigned long i;
	unsigned long ra_index = 0;
	int ret;
	int defrag_count = 0;
	int compress_type = BTRFS_COMPRESS_ZLIB;
	int extent_thresh = range->extent_thresh;
	int max_cluster = (256 * 1024) >> PAGE_CACHE_SHIFT;
	int cluster = max_cluster;
	u64 new_align = ~((u64)128 * 1024 - 1);
	struct page **pages = NULL;

	if (extent_thresh == 0)
		extent_thresh = 256 * 1024;

	if (range->flags & BTRFS_DEFRAG_RANGE_COMPRESS) {
		if (range->compress_type > BTRFS_COMPRESS_TYPES)
			return -EINVAL;
		if (range->compress_type)
			compress_type = range->compress_type;
	}

	if (isize == 0)
		return 0;

	 
	if (!file) {
		ra = kzalloc(sizeof(*ra), GFP_NOFS);
		if (!ra)
			return -ENOMEM;
		file_ra_state_init(ra, inode->i_mapping);
	} else {
		ra = &file->f_ra;
	}

	pages = kmalloc(sizeof(struct page *) * max_cluster,
			GFP_NOFS);
	if (!pages) {
		ret = -ENOMEM;
		goto out_ra;
	}

	 
	if (range->start + range->len > range->start) {
		last_index = min_t(u64, isize - 1,
			 range->start + range->len - 1) >> PAGE_CACHE_SHIFT;
	} else {
		last_index = (isize - 1) >> PAGE_CACHE_SHIFT;
	}

	if (newer_than) {
		ret = find_new_extents(root, inode, newer_than,
				       &newer_off, 64 * 1024);
		if (!ret) {
			range->start = newer_off;
			 
			i = (newer_off & new_align) >> PAGE_CACHE_SHIFT;
		} else
			goto out_ra;
	} else {
		i = range->start >> PAGE_CACHE_SHIFT;
	}
	if (!max_to_defrag)
		max_to_defrag = last_index + 1;

	 
	if (i < inode->i_mapping->writeback_index)
		inode->i_mapping->writeback_index = i;

	while (i <= last_index && defrag_count < max_to_defrag &&
	       (i < (i_size_read(inode) + PAGE_CACHE_SIZE - 1) >>
		PAGE_CACHE_SHIFT)) {
		 
		if (!(inode->i_sb->s_flags & MS_ACTIVE))
			break;

		if (!should_defrag_range(inode, (u64)i << PAGE_CACHE_SHIFT,
					 extent_thresh, &last_len, &skip,
					 &defrag_end, range->flags &
					 BTRFS_DEFRAG_RANGE_COMPRESS)) {
			unsigned long next;
			 
			next = (skip + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
			i = max(i + 1, next);
			continue;
		}

		if (!newer_than) {
			cluster = (PAGE_CACHE_ALIGN(defrag_end) >>
				   PAGE_CACHE_SHIFT) - i;
			cluster = min(cluster, max_cluster);
		} else {
			cluster = max_cluster;
		}

		if (range->flags & BTRFS_DEFRAG_RANGE_COMPRESS)
			BTRFS_I(inode)->force_compress = compress_type;

		if (i + cluster > ra_index) {
			ra_index = max(i, ra_index);
			btrfs_force_ra(inode->i_mapping, ra, file, ra_index,
				       cluster);
			ra_index += max_cluster;
		}

		mutex_lock(&inode->i_mutex);
		ret = cluster_pages_for_defrag(inode, pages, i, cluster);
		if (ret < 0) {
			mutex_unlock(&inode->i_mutex);
			goto out_ra;
		}

		defrag_count += ret;
		balance_dirty_pages_ratelimited_nr(inode->i_mapping, ret);
		mutex_unlock(&inode->i_mutex);

		if (newer_than) {
			if (newer_off == (u64)-1)
				break;

			if (ret > 0)
				i += ret;

			newer_off = max(newer_off + 1,
					(u64)i << PAGE_CACHE_SHIFT);

			ret = find_new_extents(root, inode,
					       newer_than, &newer_off,
					       64 * 1024);
			if (!ret) {
				range->start = newer_off;
				i = (newer_off & new_align) >> PAGE_CACHE_SHIFT;
			} else {
				break;
			}
		} else {
			if (ret > 0) {
				i += ret;
				last_len += ret << PAGE_CACHE_SHIFT;
			} else {
				i++;
				last_len = 0;
			}
		}
	}

	if ((range->flags & BTRFS_DEFRAG_RANGE_START_IO))
		filemap_flush(inode->i_mapping);

	if ((range->flags & BTRFS_DEFRAG_RANGE_COMPRESS)) {
		 
		atomic_inc(&root->fs_info->async_submit_draining);
		while (atomic_read(&root->fs_info->nr_async_submits) ||
		      atomic_read(&root->fs_info->async_delalloc_pages)) {
			wait_event(root->fs_info->async_submit_wait,
			   (atomic_read(&root->fs_info->nr_async_submits) == 0 &&
			    atomic_read(&root->fs_info->async_delalloc_pages) == 0));
		}
		atomic_dec(&root->fs_info->async_submit_draining);

		mutex_lock(&inode->i_mutex);
		BTRFS_I(inode)->force_compress = BTRFS_COMPRESS_NONE;
		mutex_unlock(&inode->i_mutex);
	}

	if (range->compress_type == BTRFS_COMPRESS_LZO) {
		btrfs_set_fs_incompat(root->fs_info, COMPRESS_LZO);
	}

	ret = defrag_count;

out_ra:
	if (!file)
		kfree(ra);
	kfree(pages);
	return ret;
}