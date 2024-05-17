static int ext4_journal_check_start(struct super_block *sb)
{
	journal_t *journal;

	might_sleep();
	if (sb->s_flags & MS_RDONLY)
		return -EROFS;
	WARN_ON(sb->s_writers.frozen == SB_FREEZE_COMPLETE);
	journal = EXT4_SB(sb)->s_journal;
	 
	if (journal && is_journal_aborted(journal)) {
		ext4_abort(sb, "Detected aborted journal");
		return -EROFS;
	}
	return 0;
}