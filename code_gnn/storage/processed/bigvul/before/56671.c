static void ext4_handle_error(struct super_block *sb)
{
	if (sb->s_flags & MS_RDONLY)
		return;

	if (!test_opt(sb, ERRORS_CONT)) {
		journal_t *journal = EXT4_SB(sb)->s_journal;

		EXT4_SB(sb)->s_mount_flags |= EXT4_MF_FS_ABORTED;
		if (journal)
			jbd2_journal_abort(journal, -EIO);
	}
	if (test_opt(sb, ERRORS_RO)) {
		ext4_msg(sb, KERN_CRIT, "Remounting filesystem read-only");
		 
		smp_wmb();
		sb->s_flags |= MS_RDONLY;
	}
	if (test_opt(sb, ERRORS_PANIC)) {
		if (EXT4_SB(sb)->s_journal &&
		  !(EXT4_SB(sb)->s_journal->j_flags & JBD2_REC_ERR))
			return;
		panic("EXT4-fs (device %s): panic forced after error\n",
			sb->s_id);
	}
}