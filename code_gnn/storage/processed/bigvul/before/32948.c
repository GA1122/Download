static int ext3_show_options(struct seq_file *seq, struct dentry *root)
{
	struct super_block *sb = root->d_sb;
	struct ext3_sb_info *sbi = EXT3_SB(sb);
	struct ext3_super_block *es = sbi->s_es;
	unsigned long def_mount_opts;

	def_mount_opts = le32_to_cpu(es->s_default_mount_opts);

	if (sbi->s_sb_block != 1)
		seq_printf(seq, ",sb=%lu", sbi->s_sb_block);
	if (test_opt(sb, MINIX_DF))
		seq_puts(seq, ",minixdf");
	if (test_opt(sb, GRPID))
		seq_puts(seq, ",grpid");
	if (!test_opt(sb, GRPID) && (def_mount_opts & EXT3_DEFM_BSDGROUPS))
		seq_puts(seq, ",nogrpid");
	if (!uid_eq(sbi->s_resuid, make_kuid(&init_user_ns, EXT3_DEF_RESUID)) ||
	    le16_to_cpu(es->s_def_resuid) != EXT3_DEF_RESUID) {
		seq_printf(seq, ",resuid=%u",
				from_kuid_munged(&init_user_ns, sbi->s_resuid));
	}
	if (!gid_eq(sbi->s_resgid, make_kgid(&init_user_ns, EXT3_DEF_RESGID)) ||
	    le16_to_cpu(es->s_def_resgid) != EXT3_DEF_RESGID) {
		seq_printf(seq, ",resgid=%u",
				from_kgid_munged(&init_user_ns, sbi->s_resgid));
	}
	if (test_opt(sb, ERRORS_RO)) {
		int def_errors = le16_to_cpu(es->s_errors);

		if (def_errors == EXT3_ERRORS_PANIC ||
		    def_errors == EXT3_ERRORS_CONTINUE) {
			seq_puts(seq, ",errors=remount-ro");
		}
	}
	if (test_opt(sb, ERRORS_CONT))
		seq_puts(seq, ",errors=continue");
	if (test_opt(sb, ERRORS_PANIC))
		seq_puts(seq, ",errors=panic");
	if (test_opt(sb, NO_UID32))
		seq_puts(seq, ",nouid32");
	if (test_opt(sb, DEBUG))
		seq_puts(seq, ",debug");
#ifdef CONFIG_EXT3_FS_XATTR
	if (test_opt(sb, XATTR_USER))
		seq_puts(seq, ",user_xattr");
	if (!test_opt(sb, XATTR_USER) &&
	    (def_mount_opts & EXT3_DEFM_XATTR_USER)) {
		seq_puts(seq, ",nouser_xattr");
	}
#endif
#ifdef CONFIG_EXT3_FS_POSIX_ACL
	if (test_opt(sb, POSIX_ACL))
		seq_puts(seq, ",acl");
	if (!test_opt(sb, POSIX_ACL) && (def_mount_opts & EXT3_DEFM_ACL))
		seq_puts(seq, ",noacl");
#endif
	if (!test_opt(sb, RESERVATION))
		seq_puts(seq, ",noreservation");
	if (sbi->s_commit_interval) {
		seq_printf(seq, ",commit=%u",
			   (unsigned) (sbi->s_commit_interval / HZ));
	}

	 
	seq_puts(seq, ",barrier=");
	seq_puts(seq, test_opt(sb, BARRIER) ? "1" : "0");
	seq_printf(seq, ",data=%s", data_mode_string(test_opt(sb, DATA_FLAGS)));
	if (test_opt(sb, DATA_ERR_ABORT))
		seq_puts(seq, ",data_err=abort");

	if (test_opt(sb, NOLOAD))
		seq_puts(seq, ",norecovery");

	ext3_show_quota_options(seq, sb);

	return 0;
}