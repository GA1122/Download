static int ext2_remount (struct super_block * sb, int * flags, char * data)
{
	struct ext2_sb_info * sbi = EXT2_SB(sb);
	struct ext2_super_block * es;
	struct ext2_mount_options old_opts;
	unsigned long old_sb_flags;
	int err;

	sync_filesystem(sb);
	spin_lock(&sbi->s_lock);

	 
	old_sb_flags = sb->s_flags;
	old_opts.s_mount_opt = sbi->s_mount_opt;
	old_opts.s_resuid = sbi->s_resuid;
	old_opts.s_resgid = sbi->s_resgid;

	 
	if (!parse_options(data, sb)) {
		err = -EINVAL;
		goto restore_opts;
	}

	sb->s_flags = (sb->s_flags & ~MS_POSIXACL) |
		((sbi->s_mount_opt & EXT2_MOUNT_POSIX_ACL) ? MS_POSIXACL : 0);

	es = sbi->s_es;
	if ((sbi->s_mount_opt ^ old_opts.s_mount_opt) & EXT2_MOUNT_DAX) {
		ext2_msg(sb, KERN_WARNING, "warning: refusing change of "
			 "dax flag with busy inodes while remounting");
		sbi->s_mount_opt ^= EXT2_MOUNT_DAX;
	}
	if ((*flags & MS_RDONLY) == (sb->s_flags & MS_RDONLY)) {
		spin_unlock(&sbi->s_lock);
		return 0;
	}
	if (*flags & MS_RDONLY) {
		if (le16_to_cpu(es->s_state) & EXT2_VALID_FS ||
		    !(sbi->s_mount_state & EXT2_VALID_FS)) {
			spin_unlock(&sbi->s_lock);
			return 0;
		}

		 
		es->s_state = cpu_to_le16(sbi->s_mount_state);
		es->s_mtime = cpu_to_le32(get_seconds());
		spin_unlock(&sbi->s_lock);

		err = dquot_suspend(sb, -1);
		if (err < 0) {
			spin_lock(&sbi->s_lock);
			goto restore_opts;
		}

		ext2_sync_super(sb, es, 1);
	} else {
		__le32 ret = EXT2_HAS_RO_COMPAT_FEATURE(sb,
					       ~EXT2_FEATURE_RO_COMPAT_SUPP);
		if (ret) {
			ext2_msg(sb, KERN_WARNING,
				"warning: couldn't remount RDWR because of "
				"unsupported optional features (%x).",
				le32_to_cpu(ret));
			err = -EROFS;
			goto restore_opts;
		}
		 
		sbi->s_mount_state = le16_to_cpu(es->s_state);
		if (!ext2_setup_super (sb, es, 0))
			sb->s_flags &= ~MS_RDONLY;
		spin_unlock(&sbi->s_lock);

		ext2_write_super(sb);

		dquot_resume(sb, -1);
	}

	return 0;
restore_opts:
	sbi->s_mount_opt = old_opts.s_mount_opt;
	sbi->s_resuid = old_opts.s_resuid;
	sbi->s_resgid = old_opts.s_resgid;
	sb->s_flags = old_sb_flags;
	spin_unlock(&sbi->s_lock);
	return err;
}
