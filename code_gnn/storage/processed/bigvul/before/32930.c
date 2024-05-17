static void ext3_init_journal_params(struct super_block *sb, journal_t *journal)
{
	struct ext3_sb_info *sbi = EXT3_SB(sb);

	if (sbi->s_commit_interval)
		journal->j_commit_interval = sbi->s_commit_interval;
	 

	spin_lock(&journal->j_state_lock);
	if (test_opt(sb, BARRIER))
		journal->j_flags |= JFS_BARRIER;
	else
		journal->j_flags &= ~JFS_BARRIER;
	if (test_opt(sb, DATA_ERR_ABORT))
		journal->j_flags |= JFS_ABORT_ON_SYNCDATA_ERR;
	else
		journal->j_flags &= ~JFS_ABORT_ON_SYNCDATA_ERR;
	spin_unlock(&journal->j_state_lock);
}