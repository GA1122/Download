static int mpage_map_and_submit_extent(handle_t *handle,
				       struct mpage_da_data *mpd,
				       bool *give_up_on_write)
{
	struct inode *inode = mpd->inode;
	struct ext4_map_blocks *map = &mpd->map;
	int err;
	loff_t disksize;
	int progress = 0;

	mpd->io_submit.io_end->offset =
				((loff_t)map->m_lblk) << inode->i_blkbits;
	do {
		err = mpage_map_one_extent(handle, mpd);
		if (err < 0) {
			struct super_block *sb = inode->i_sb;

			if (EXT4_SB(sb)->s_mount_flags & EXT4_MF_FS_ABORTED)
				goto invalidate_dirty_pages;
			 
			if ((err == -ENOMEM) ||
			    (err == -ENOSPC && ext4_count_free_clusters(sb))) {
				if (progress)
					goto update_disksize;
				return err;
			}
			ext4_msg(sb, KERN_CRIT,
				 "Delayed block allocation failed for "
				 "inode %lu at logical offset %llu with"
				 " max blocks %u with error %d",
				 inode->i_ino,
				 (unsigned long long)map->m_lblk,
				 (unsigned)map->m_len, -err);
			ext4_msg(sb, KERN_CRIT,
				 "This should not happen!! Data will "
				 "be lost\n");
			if (err == -ENOSPC)
				ext4_print_free_blocks(inode);
		invalidate_dirty_pages:
			*give_up_on_write = true;
			return err;
		}
		progress = 1;
		 
		err = mpage_map_and_submit_buffers(mpd);
		if (err < 0)
			goto update_disksize;
	} while (map->m_len);

update_disksize:
	 
	disksize = ((loff_t)mpd->first_page) << PAGE_SHIFT;
	if (disksize > EXT4_I(inode)->i_disksize) {
		int err2;
		loff_t i_size;

		down_write(&EXT4_I(inode)->i_data_sem);
		i_size = i_size_read(inode);
		if (disksize > i_size)
			disksize = i_size;
		if (disksize > EXT4_I(inode)->i_disksize)
			EXT4_I(inode)->i_disksize = disksize;
		err2 = ext4_mark_inode_dirty(handle, inode);
		up_write(&EXT4_I(inode)->i_data_sem);
		if (err2)
			ext4_error(inode->i_sb,
				   "Failed to mark inode %lu dirty",
				   inode->i_ino);
		if (!err)
			err = err2;
	}
	return err;
}