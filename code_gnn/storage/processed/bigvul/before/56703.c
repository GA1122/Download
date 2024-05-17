static int ext4_unfreeze(struct super_block *sb)
{
	if (sb->s_flags & MS_RDONLY)
		return 0;

	if (EXT4_SB(sb)->s_journal) {
		 
		ext4_set_feature_journal_needs_recovery(sb);
	}

	ext4_commit_super(sb, 1);
	return 0;
}