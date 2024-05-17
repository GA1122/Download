static int ext3_create_journal(struct super_block *sb,
			       struct ext3_super_block *es,
			       unsigned int journal_inum)
{
	journal_t *journal;
	int err;

	if (sb->s_flags & MS_RDONLY) {
		ext3_msg(sb, KERN_ERR,
			"error: readonly filesystem when trying to "
			"create journal");
		return -EROFS;
	}

	journal = ext3_get_journal(sb, journal_inum);
	if (!journal)
		return -EINVAL;

	ext3_msg(sb, KERN_INFO, "creating new journal on inode %u",
	       journal_inum);

	err = journal_create(journal);
	if (err) {
		ext3_msg(sb, KERN_ERR, "error creating journal");
		journal_destroy(journal);
		return -EIO;
	}

	EXT3_SB(sb)->s_journal = journal;

	ext3_update_dynamic_rev(sb);
	EXT3_SET_INCOMPAT_FEATURE(sb, EXT3_FEATURE_INCOMPAT_RECOVER);
	EXT3_SET_COMPAT_FEATURE(sb, EXT3_FEATURE_COMPAT_HAS_JOURNAL);

	es->s_journal_inum = cpu_to_le32(journal_inum);

	 
	ext3_commit_super(sb, es, 1);

	return 0;
}