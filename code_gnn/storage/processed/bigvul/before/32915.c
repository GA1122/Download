static void ext3_clear_journal_err(struct super_block *sb,
				   struct ext3_super_block *es)
{
	journal_t *journal;
	int j_errno;
	const char *errstr;

	journal = EXT3_SB(sb)->s_journal;

	 

	j_errno = journal_errno(journal);
	if (j_errno) {
		char nbuf[16];

		errstr = ext3_decode_error(sb, j_errno, nbuf);
		ext3_warning(sb, __func__, "Filesystem error recorded "
			     "from previous mount: %s", errstr);
		ext3_warning(sb, __func__, "Marking fs in need of "
			     "filesystem check.");

		EXT3_SB(sb)->s_mount_state |= EXT3_ERROR_FS;
		es->s_state |= cpu_to_le16(EXT3_ERROR_FS);
		ext3_commit_super (sb, es, 1);

		journal_clear_err(journal);
	}
}