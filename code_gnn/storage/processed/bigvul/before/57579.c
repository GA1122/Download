static int ext4_quota_on(struct super_block *sb, int type, int format_id,
			 char *name, int remount)
{
	int err;
	struct path path;

	if (!test_opt(sb, QUOTA))
		return -EINVAL;
	 
	if (remount)
		return vfs_quota_on(sb, type, format_id, name, remount);

	err = kern_path(name, LOOKUP_FOLLOW, &path);
	if (err)
		return err;

	 
	if (path.mnt->mnt_sb != sb) {
		path_put(&path);
		return -EXDEV;
	}
	 
	if (EXT4_SB(sb)->s_qf_names[type]) {
		 
		if (path.dentry->d_parent != sb->s_root)
			ext4_msg(sb, KERN_WARNING,
				"Quota file not on filesystem root. "
				"Journaled quota will not work");
	}

	 
	if (EXT4_SB(sb)->s_journal &&
	    ext4_should_journal_data(path.dentry->d_inode)) {
		 
		jbd2_journal_lock_updates(EXT4_SB(sb)->s_journal);
		err = jbd2_journal_flush(EXT4_SB(sb)->s_journal);
		jbd2_journal_unlock_updates(EXT4_SB(sb)->s_journal);
		if (err) {
			path_put(&path);
			return err;
		}
	}

	err = vfs_quota_on_path(sb, type, format_id, &path);
	path_put(&path);
	return err;
}
