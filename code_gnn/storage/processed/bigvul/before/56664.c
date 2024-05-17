static int ext4_freeze(struct super_block *sb)
{
	int error = 0;
	journal_t *journal;

	if (sb->s_flags & MS_RDONLY)
		return 0;

	journal = EXT4_SB(sb)->s_journal;

	if (journal) {
		 
		jbd2_journal_lock_updates(journal);

		 
		error = jbd2_journal_flush(journal);
		if (error < 0)
			goto out;

		 
		ext4_clear_feature_journal_needs_recovery(sb);
	}

	error = ext4_commit_super(sb, 1);
out:
	if (journal)
		 
		jbd2_journal_unlock_updates(journal);
	return error;
}
