int ext4_write_inode(struct inode *inode, struct writeback_control *wbc)
{
	int err;

	if (WARN_ON_ONCE(current->flags & PF_MEMALLOC))
		return 0;

	if (EXT4_SB(inode->i_sb)->s_journal) {
		if (ext4_journal_current_handle()) {
			jbd_debug(1, "called recursively, non-PF_MEMALLOC!\n");
			dump_stack();
			return -EIO;
		}

		 
		if (wbc->sync_mode != WB_SYNC_ALL || wbc->for_sync)
			return 0;

		err = ext4_force_commit(inode->i_sb);
	} else {
		struct ext4_iloc iloc;

		err = __ext4_get_inode_loc(inode, &iloc, 0);
		if (err)
			return err;
		 
		if (wbc->sync_mode == WB_SYNC_ALL && !wbc->for_sync)
			sync_dirty_buffer(iloc.bh);
		if (buffer_req(iloc.bh) && !buffer_uptodate(iloc.bh)) {
			EXT4_ERROR_INODE_BLOCK(inode, iloc.bh->b_blocknr,
					 "IO error syncing inode");
			err = -EIO;
		}
		brelse(iloc.bh);
	}
	return err;
}