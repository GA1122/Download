static int ext3_remount (struct super_block * sb, int * flags, char * data)
{
	struct ext3_super_block * es;
	struct ext3_sb_info *sbi = EXT3_SB(sb);
	ext3_fsblk_t n_blocks_count = 0;
	unsigned long old_sb_flags;
	struct ext3_mount_options old_opts;
	int enable_quota = 0;
	int err;
#ifdef CONFIG_QUOTA
	int i;
#endif

	 
	old_sb_flags = sb->s_flags;
	old_opts.s_mount_opt = sbi->s_mount_opt;
	old_opts.s_resuid = sbi->s_resuid;
	old_opts.s_resgid = sbi->s_resgid;
	old_opts.s_commit_interval = sbi->s_commit_interval;
#ifdef CONFIG_QUOTA
	old_opts.s_jquota_fmt = sbi->s_jquota_fmt;
	for (i = 0; i < MAXQUOTAS; i++)
		if (sbi->s_qf_names[i]) {
			old_opts.s_qf_names[i] = kstrdup(sbi->s_qf_names[i],
							 GFP_KERNEL);
			if (!old_opts.s_qf_names[i]) {
				int j;

				for (j = 0; j < i; j++)
					kfree(old_opts.s_qf_names[j]);
				return -ENOMEM;
			}
		} else
			old_opts.s_qf_names[i] = NULL;
#endif

	 
	if (!parse_options(data, sb, NULL, NULL, &n_blocks_count, 1)) {
		err = -EINVAL;
		goto restore_opts;
	}

	if (test_opt(sb, ABORT))
		ext3_abort(sb, __func__, "Abort forced by user");

	sb->s_flags = (sb->s_flags & ~MS_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? MS_POSIXACL : 0);

	es = sbi->s_es;

	ext3_init_journal_params(sb, sbi->s_journal);

	if ((*flags & MS_RDONLY) != (sb->s_flags & MS_RDONLY) ||
		n_blocks_count > le32_to_cpu(es->s_blocks_count)) {
		if (test_opt(sb, ABORT)) {
			err = -EROFS;
			goto restore_opts;
		}

		if (*flags & MS_RDONLY) {
			err = dquot_suspend(sb, -1);
			if (err < 0)
				goto restore_opts;

			 
			sb->s_flags |= MS_RDONLY;

			 
			if (!(es->s_state & cpu_to_le16(EXT3_VALID_FS)) &&
			    (sbi->s_mount_state & EXT3_VALID_FS))
				es->s_state = cpu_to_le16(sbi->s_mount_state);

			ext3_mark_recovery_complete(sb, es);
		} else {
			__le32 ret;
			if ((ret = EXT3_HAS_RO_COMPAT_FEATURE(sb,
					~EXT3_FEATURE_RO_COMPAT_SUPP))) {
				ext3_msg(sb, KERN_WARNING,
					"warning: couldn't remount RDWR "
					"because of unsupported optional "
					"features (%x)", le32_to_cpu(ret));
				err = -EROFS;
				goto restore_opts;
			}

			 
			if (es->s_last_orphan) {
				ext3_msg(sb, KERN_WARNING, "warning: couldn't "
				       "remount RDWR because of unprocessed "
				       "orphan inode list.  Please "
				       "umount & mount instead.");
				err = -EINVAL;
				goto restore_opts;
			}

			 
			ext3_clear_journal_err(sb, es);
			sbi->s_mount_state = le16_to_cpu(es->s_state);
			if ((err = ext3_group_extend(sb, es, n_blocks_count)))
				goto restore_opts;
			if (!ext3_setup_super (sb, es, 0))
				sb->s_flags &= ~MS_RDONLY;
			enable_quota = 1;
		}
	}
#ifdef CONFIG_QUOTA
	 
	for (i = 0; i < MAXQUOTAS; i++)
		kfree(old_opts.s_qf_names[i]);
#endif
	if (enable_quota)
		dquot_resume(sb, -1);
	return 0;
restore_opts:
	sb->s_flags = old_sb_flags;
	sbi->s_mount_opt = old_opts.s_mount_opt;
	sbi->s_resuid = old_opts.s_resuid;
	sbi->s_resgid = old_opts.s_resgid;
	sbi->s_commit_interval = old_opts.s_commit_interval;
#ifdef CONFIG_QUOTA
	sbi->s_jquota_fmt = old_opts.s_jquota_fmt;
	for (i = 0; i < MAXQUOTAS; i++) {
		kfree(sbi->s_qf_names[i]);
		sbi->s_qf_names[i] = old_opts.s_qf_names[i];
	}
#endif
	return err;
}
