static int set_journal_csum_feature_set(struct super_block *sb)
{
	int ret = 1;
	int compat, incompat;
	struct ext4_sb_info *sbi = EXT4_SB(sb);

	if (ext4_has_metadata_csum(sb)) {
		 
		compat = 0;
		incompat = JBD2_FEATURE_INCOMPAT_CSUM_V3;
	} else {
		 
		compat = JBD2_FEATURE_COMPAT_CHECKSUM;
		incompat = 0;
	}

	jbd2_journal_clear_features(sbi->s_journal,
			JBD2_FEATURE_COMPAT_CHECKSUM, 0,
			JBD2_FEATURE_INCOMPAT_CSUM_V3 |
			JBD2_FEATURE_INCOMPAT_CSUM_V2);
	if (test_opt(sb, JOURNAL_ASYNC_COMMIT)) {
		ret = jbd2_journal_set_features(sbi->s_journal,
				compat, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT |
				incompat);
	} else if (test_opt(sb, JOURNAL_CHECKSUM)) {
		ret = jbd2_journal_set_features(sbi->s_journal,
				compat, 0,
				incompat);
		jbd2_journal_clear_features(sbi->s_journal, 0, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
	} else {
		jbd2_journal_clear_features(sbi->s_journal, 0, 0,
				JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT);
	}

	return ret;
}