static int ext3_unfreeze(struct super_block *sb)
{
	if (!(sb->s_flags & MS_RDONLY)) {
		 
		EXT3_SET_INCOMPAT_FEATURE(sb, EXT3_FEATURE_INCOMPAT_RECOVER);
		ext3_commit_super(sb, EXT3_SB(sb)->s_es, 1);
		journal_unlock_updates(EXT3_SB(sb)->s_journal);
	}
	return 0;
}