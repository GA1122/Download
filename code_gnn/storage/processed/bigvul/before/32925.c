static int ext3_freeze(struct super_block *sb)
{
	int error = 0;
	journal_t *journal;

	if (!(sb->s_flags & MS_RDONLY)) {
		journal = EXT3_SB(sb)->s_journal;

		 
		journal_lock_updates(journal);

		 
		error = journal_flush(journal);
		if (error < 0)
			goto out;

		 
		EXT3_CLEAR_INCOMPAT_FEATURE(sb, EXT3_FEATURE_INCOMPAT_RECOVER);
		error = ext3_commit_super(sb, EXT3_SB(sb)->s_es, 1);
		if (error)
			goto out;
	}
	return 0;

out:
	journal_unlock_updates(journal);
	return error;
}
