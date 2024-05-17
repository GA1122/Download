int ext4_change_inode_journal_flag(struct inode *inode, int val)
{
	journal_t *journal;
	handle_t *handle;
	int err;

	 

	journal = EXT4_JOURNAL(inode);
	if (!journal)
		return 0;
	if (is_journal_aborted(journal))
		return -EROFS;

	jbd2_journal_lock_updates(journal);
	jbd2_journal_flush(journal);

	 

	if (val)
		EXT4_I(inode)->i_flags |= EXT4_JOURNAL_DATA_FL;
	else
		EXT4_I(inode)->i_flags &= ~EXT4_JOURNAL_DATA_FL;
	ext4_set_aops(inode);

	jbd2_journal_unlock_updates(journal);

	 

	handle = ext4_journal_start(inode, 1);
	if (IS_ERR(handle))
		return PTR_ERR(handle);

	err = ext4_mark_inode_dirty(handle, inode);
	ext4_handle_sync(handle);
	ext4_journal_stop(handle);
	ext4_std_error(inode->i_sb, err);

	return err;
}
