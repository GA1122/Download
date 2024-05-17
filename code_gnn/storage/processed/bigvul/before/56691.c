static int ext4_remount(struct super_block *sb, int *flags, char *data)
{
	struct ext4_super_block *es;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	unsigned long old_sb_flags;
	struct ext4_mount_options old_opts;
	int enable_quota = 0;
	ext4_group_t g;
	unsigned int journal_ioprio = DEFAULT_JOURNAL_IOPRIO;
	int err = 0;
#ifdef CONFIG_QUOTA
	int i, j;
#endif
	char *orig_data = kstrdup(data, GFP_KERNEL);

	 
	old_sb_flags = sb->s_flags;
	old_opts.s_mount_opt = sbi->s_mount_opt;
	old_opts.s_mount_opt2 = sbi->s_mount_opt2;
	old_opts.s_resuid = sbi->s_resuid;
	old_opts.s_resgid = sbi->s_resgid;
	old_opts.s_commit_interval = sbi->s_commit_interval;
	old_opts.s_min_batch_time = sbi->s_min_batch_time;
	old_opts.s_max_batch_time = sbi->s_max_batch_time;
#ifdef CONFIG_QUOTA
	old_opts.s_jquota_fmt = sbi->s_jquota_fmt;
	for (i = 0; i < EXT4_MAXQUOTAS; i++)
		if (sbi->s_qf_names[i]) {
			old_opts.s_qf_names[i] = kstrdup(sbi->s_qf_names[i],
							 GFP_KERNEL);
			if (!old_opts.s_qf_names[i]) {
				for (j = 0; j < i; j++)
					kfree(old_opts.s_qf_names[j]);
				kfree(orig_data);
				return -ENOMEM;
			}
		} else
			old_opts.s_qf_names[i] = NULL;
#endif
	if (sbi->s_journal && sbi->s_journal->j_task->io_context)
		journal_ioprio = sbi->s_journal->j_task->io_context->ioprio;

	if (!parse_options(data, sb, NULL, &journal_ioprio, 1)) {
		err = -EINVAL;
		goto restore_opts;
	}

	if ((old_opts.s_mount_opt & EXT4_MOUNT_JOURNAL_CHECKSUM) ^
	    test_opt(sb, JOURNAL_CHECKSUM)) {
		ext4_msg(sb, KERN_ERR, "changing journal_checksum "
			 "during remount not supported; ignoring");
		sbi->s_mount_opt ^= EXT4_MOUNT_JOURNAL_CHECKSUM;
	}

	if (test_opt(sb, DATA_FLAGS) == EXT4_MOUNT_JOURNAL_DATA) {
		if (test_opt2(sb, EXPLICIT_DELALLOC)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and delalloc");
			err = -EINVAL;
			goto restore_opts;
		}
		if (test_opt(sb, DIOREAD_NOLOCK)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and dioread_nolock");
			err = -EINVAL;
			goto restore_opts;
		}
		if (test_opt(sb, DAX)) {
			ext4_msg(sb, KERN_ERR, "can't mount with "
				 "both data=journal and dax");
			err = -EINVAL;
			goto restore_opts;
		}
	}

	if ((sbi->s_mount_opt ^ old_opts.s_mount_opt) & EXT4_MOUNT_DAX) {
		ext4_msg(sb, KERN_WARNING, "warning: refusing change of "
			"dax flag with busy inodes while remounting");
		sbi->s_mount_opt ^= EXT4_MOUNT_DAX;
	}

	if (sbi->s_mount_flags & EXT4_MF_FS_ABORTED)
		ext4_abort(sb, "Abort forced by user");

	sb->s_flags = (sb->s_flags & ~MS_POSIXACL) |
		(test_opt(sb, POSIX_ACL) ? MS_POSIXACL : 0);

	es = sbi->s_es;

	if (sbi->s_journal) {
		ext4_init_journal_params(sb, sbi->s_journal);
		set_task_ioprio(sbi->s_journal->j_task, journal_ioprio);
	}

	if (*flags & MS_LAZYTIME)
		sb->s_flags |= MS_LAZYTIME;

	if ((*flags & MS_RDONLY) != (sb->s_flags & MS_RDONLY)) {
		if (sbi->s_mount_flags & EXT4_MF_FS_ABORTED) {
			err = -EROFS;
			goto restore_opts;
		}

		if (*flags & MS_RDONLY) {
			err = sync_filesystem(sb);
			if (err < 0)
				goto restore_opts;
			err = dquot_suspend(sb, -1);
			if (err < 0)
				goto restore_opts;

			 
			sb->s_flags |= MS_RDONLY;

			 
			if (!(es->s_state & cpu_to_le16(EXT4_VALID_FS)) &&
			    (sbi->s_mount_state & EXT4_VALID_FS))
				es->s_state = cpu_to_le16(sbi->s_mount_state);

			if (sbi->s_journal)
				ext4_mark_recovery_complete(sb, es);
		} else {
			 
			if (ext4_has_feature_readonly(sb) ||
			    !ext4_feature_set_ok(sb, 0)) {
				err = -EROFS;
				goto restore_opts;
			}
			 
			for (g = 0; g < sbi->s_groups_count; g++) {
				struct ext4_group_desc *gdp =
					ext4_get_group_desc(sb, g, NULL);

				if (!ext4_group_desc_csum_verify(sb, g, gdp)) {
					ext4_msg(sb, KERN_ERR,
	       "ext4_remount: Checksum for group %u failed (%u!=%u)",
		g, le16_to_cpu(ext4_group_desc_csum(sb, g, gdp)),
					       le16_to_cpu(gdp->bg_checksum));
					err = -EFSBADCRC;
					goto restore_opts;
				}
			}

			 
			if (es->s_last_orphan) {
				ext4_msg(sb, KERN_WARNING, "Couldn't "
				       "remount RDWR because of unprocessed "
				       "orphan inode list.  Please "
				       "umount/remount instead");
				err = -EINVAL;
				goto restore_opts;
			}

			 
			if (sbi->s_journal)
				ext4_clear_journal_err(sb, es);
			sbi->s_mount_state = le16_to_cpu(es->s_state);
			if (!ext4_setup_super(sb, es, 0))
				sb->s_flags &= ~MS_RDONLY;
			if (ext4_has_feature_mmp(sb))
				if (ext4_multi_mount_protect(sb,
						le64_to_cpu(es->s_mmp_block))) {
					err = -EROFS;
					goto restore_opts;
				}
			enable_quota = 1;
		}
	}

	 
	if ((sb->s_flags & MS_RDONLY) || !test_opt(sb, INIT_INODE_TABLE))
		ext4_unregister_li_request(sb);
	else {
		ext4_group_t first_not_zeroed;
		first_not_zeroed = ext4_has_uninit_itable(sb);
		ext4_register_li_request(sb, first_not_zeroed);
	}

	ext4_setup_system_zone(sb);
	if (sbi->s_journal == NULL && !(old_sb_flags & MS_RDONLY))
		ext4_commit_super(sb, 1);

#ifdef CONFIG_QUOTA
	 
	for (i = 0; i < EXT4_MAXQUOTAS; i++)
		kfree(old_opts.s_qf_names[i]);
	if (enable_quota) {
		if (sb_any_quota_suspended(sb))
			dquot_resume(sb, -1);
		else if (ext4_has_feature_quota(sb)) {
			err = ext4_enable_quotas(sb);
			if (err)
				goto restore_opts;
		}
	}
#endif

	*flags = (*flags & ~MS_LAZYTIME) | (sb->s_flags & MS_LAZYTIME);
	ext4_msg(sb, KERN_INFO, "re-mounted. Opts: %s", orig_data);
	kfree(orig_data);
	return 0;

restore_opts:
	sb->s_flags = old_sb_flags;
	sbi->s_mount_opt = old_opts.s_mount_opt;
	sbi->s_mount_opt2 = old_opts.s_mount_opt2;
	sbi->s_resuid = old_opts.s_resuid;
	sbi->s_resgid = old_opts.s_resgid;
	sbi->s_commit_interval = old_opts.s_commit_interval;
	sbi->s_min_batch_time = old_opts.s_min_batch_time;
	sbi->s_max_batch_time = old_opts.s_max_batch_time;
#ifdef CONFIG_QUOTA
	sbi->s_jquota_fmt = old_opts.s_jquota_fmt;
	for (i = 0; i < EXT4_MAXQUOTAS; i++) {
		kfree(sbi->s_qf_names[i]);
		sbi->s_qf_names[i] = old_opts.s_qf_names[i];
	}
#endif
	kfree(orig_data);
	return err;
}