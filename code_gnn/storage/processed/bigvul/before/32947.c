static int ext3_setup_super(struct super_block *sb, struct ext3_super_block *es,
			    int read_only)
{
	struct ext3_sb_info *sbi = EXT3_SB(sb);
	int res = 0;

	if (le32_to_cpu(es->s_rev_level) > EXT3_MAX_SUPP_REV) {
		ext3_msg(sb, KERN_ERR,
			"error: revision level too high, "
			"forcing read-only mode");
		res = MS_RDONLY;
	}
	if (read_only)
		return res;
	if (!(sbi->s_mount_state & EXT3_VALID_FS))
		ext3_msg(sb, KERN_WARNING,
			"warning: mounting unchecked fs, "
			"running e2fsck is recommended");
	else if ((sbi->s_mount_state & EXT3_ERROR_FS))
		ext3_msg(sb, KERN_WARNING,
			"warning: mounting fs with errors, "
			"running e2fsck is recommended");
	else if ((__s16) le16_to_cpu(es->s_max_mnt_count) > 0 &&
		 le16_to_cpu(es->s_mnt_count) >=
			le16_to_cpu(es->s_max_mnt_count))
		ext3_msg(sb, KERN_WARNING,
			"warning: maximal mount count reached, "
			"running e2fsck is recommended");
	else if (le32_to_cpu(es->s_checkinterval) &&
		(le32_to_cpu(es->s_lastcheck) +
			le32_to_cpu(es->s_checkinterval) <= get_seconds()))
		ext3_msg(sb, KERN_WARNING,
			"warning: checktime reached, "
			"running e2fsck is recommended");
#if 0
		 
	es->s_state &= cpu_to_le16(~EXT3_VALID_FS);
#endif
	if (!le16_to_cpu(es->s_max_mnt_count))
		es->s_max_mnt_count = cpu_to_le16(EXT3_DFL_MAX_MNT_COUNT);
	le16_add_cpu(&es->s_mnt_count, 1);
	es->s_mtime = cpu_to_le32(get_seconds());
	ext3_update_dynamic_rev(sb);
	EXT3_SET_INCOMPAT_FEATURE(sb, EXT3_FEATURE_INCOMPAT_RECOVER);

	ext3_commit_super(sb, es, 1);
	if (test_opt(sb, DEBUG))
		ext3_msg(sb, KERN_INFO, "[bs=%lu, gc=%lu, "
				"bpg=%lu, ipg=%lu, mo=%04lx]",
			sb->s_blocksize,
			sbi->s_groups_count,
			EXT3_BLOCKS_PER_GROUP(sb),
			EXT3_INODES_PER_GROUP(sb),
			sbi->s_mount_opt);

	if (EXT3_SB(sb)->s_journal->j_inode == NULL) {
		char b[BDEVNAME_SIZE];
		ext3_msg(sb, KERN_INFO, "using external journal on %s",
			bdevname(EXT3_SB(sb)->s_journal->j_dev, b));
	} else {
		ext3_msg(sb, KERN_INFO, "using internal journal");
	}
	cleancache_init_fs(sb);
	return res;
}