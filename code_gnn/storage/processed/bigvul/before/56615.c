static int ext4_writepages(struct address_space *mapping,
			   struct writeback_control *wbc)
{
	pgoff_t	writeback_index = 0;
	long nr_to_write = wbc->nr_to_write;
	int range_whole = 0;
	int cycled = 1;
	handle_t *handle = NULL;
	struct mpage_da_data mpd;
	struct inode *inode = mapping->host;
	int needed_blocks, rsv_blocks = 0, ret = 0;
	struct ext4_sb_info *sbi = EXT4_SB(mapping->host->i_sb);
	bool done;
	struct blk_plug plug;
	bool give_up_on_write = false;

	trace_ext4_writepages(inode, wbc);

	 
	if (!mapping->nrpages || !mapping_tagged(mapping, PAGECACHE_TAG_DIRTY))
		goto out_writepages;

	if (ext4_should_journal_data(inode)) {
		struct blk_plug plug;

		blk_start_plug(&plug);
		ret = write_cache_pages(mapping, wbc, __writepage, mapping);
		blk_finish_plug(&plug);
		goto out_writepages;
	}

	 
	if (unlikely(sbi->s_mount_flags & EXT4_MF_FS_ABORTED)) {
		ret = -EROFS;
		goto out_writepages;
	}

	if (ext4_should_dioread_nolock(inode)) {
		 
		rsv_blocks = 1 + (PAGE_CACHE_SIZE >> inode->i_blkbits);
	}

	 
	if (ext4_has_inline_data(inode)) {
		 
		handle = ext4_journal_start(inode, EXT4_HT_INODE, 1);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			goto out_writepages;
		}
		BUG_ON(ext4_test_inode_state(inode,
				EXT4_STATE_MAY_INLINE_DATA));
		ext4_destroy_inline_data(handle, inode);
		ext4_journal_stop(handle);
	}

	if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX)
		range_whole = 1;

	if (wbc->range_cyclic) {
		writeback_index = mapping->writeback_index;
		if (writeback_index)
			cycled = 0;
		mpd.first_page = writeback_index;
		mpd.last_page = -1;
	} else {
		mpd.first_page = wbc->range_start >> PAGE_CACHE_SHIFT;
		mpd.last_page = wbc->range_end >> PAGE_CACHE_SHIFT;
	}

	mpd.inode = inode;
	mpd.wbc = wbc;
	ext4_io_submit_init(&mpd.io_submit, wbc);
retry:
	if (wbc->sync_mode == WB_SYNC_ALL || wbc->tagged_writepages)
		tag_pages_for_writeback(mapping, mpd.first_page, mpd.last_page);
	done = false;
	blk_start_plug(&plug);
	while (!done && mpd.first_page <= mpd.last_page) {
		 
		mpd.io_submit.io_end = ext4_init_io_end(inode, GFP_KERNEL);
		if (!mpd.io_submit.io_end) {
			ret = -ENOMEM;
			break;
		}

		 
		BUG_ON(ext4_should_journal_data(inode));
		needed_blocks = ext4_da_writepages_trans_blocks(inode);

		 
		handle = ext4_journal_start_with_reserve(inode,
				EXT4_HT_WRITE_PAGE, needed_blocks, rsv_blocks);
		if (IS_ERR(handle)) {
			ret = PTR_ERR(handle);
			ext4_msg(inode->i_sb, KERN_CRIT, "%s: jbd2_start: "
			       "%ld pages, ino %lu; err %d", __func__,
				wbc->nr_to_write, inode->i_ino, ret);
			 
			ext4_put_io_end(mpd.io_submit.io_end);
			break;
		}

		trace_ext4_da_write_pages(inode, mpd.first_page, mpd.wbc);
		ret = mpage_prepare_extent_to_map(&mpd);
		if (!ret) {
			if (mpd.map.m_len)
				ret = mpage_map_and_submit_extent(handle, &mpd,
					&give_up_on_write);
			else {
				 
				done = true;
			}
		}
		ext4_journal_stop(handle);
		 
		ext4_io_submit(&mpd.io_submit);
		 
		mpage_release_unused_pages(&mpd, give_up_on_write);
		 
		ext4_put_io_end(mpd.io_submit.io_end);

		if (ret == -ENOSPC && sbi->s_journal) {
			 
			jbd2_journal_force_commit_nested(sbi->s_journal);
			ret = 0;
			continue;
		}
		 
		if (ret)
			break;
	}
	blk_finish_plug(&plug);
	if (!ret && !cycled && wbc->nr_to_write > 0) {
		cycled = 1;
		mpd.last_page = writeback_index - 1;
		mpd.first_page = 0;
		goto retry;
	}

	 
	if (wbc->range_cyclic || (range_whole && wbc->nr_to_write > 0))
		 
		mapping->writeback_index = mpd.first_page;

out_writepages:
	trace_ext4_writepages_result(inode, wbc, ret,
				     nr_to_write - wbc->nr_to_write);
	return ret;
}
