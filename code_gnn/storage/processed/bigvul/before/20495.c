handle_t *ext4_journal_start_sb(struct super_block *sb, int nblocks)
{
	journal_t *journal;
	handle_t  *handle;

	trace_ext4_journal_start(sb, nblocks, _RET_IP_);
	if (sb->s_flags & MS_RDONLY)
		return ERR_PTR(-EROFS);

	journal = EXT4_SB(sb)->s_journal;
	handle = ext4_journal_current_handle();

	 
	if (!handle)
		vfs_check_frozen(sb, SB_FREEZE_TRANS);

	if (!journal)
		return ext4_get_nojournal();
	 
	if (is_journal_aborted(journal)) {
		ext4_abort(sb, "Detected aborted journal");
		return ERR_PTR(-EROFS);
	}
	return jbd2_journal_start(journal, nblocks);
}
