static int ext3_quota_on(struct super_block *sb, int type, int format_id,
			 struct path *path)
{
	int err;

	if (!test_opt(sb, QUOTA))
		return -EINVAL;

	 
	if (path->dentry->d_sb != sb)
		return -EXDEV;
	 
	if (EXT3_SB(sb)->s_qf_names[type]) {
		 
		if (path->dentry->d_parent != sb->s_root)
			ext3_msg(sb, KERN_WARNING,
				"warning: Quota file not on filesystem root. "
				"Journaled quota will not work.");
	}

	 
	if (ext3_should_journal_data(path->dentry->d_inode)) {
		 
		journal_lock_updates(EXT3_SB(sb)->s_journal);
		err = journal_flush(EXT3_SB(sb)->s_journal);
		journal_unlock_updates(EXT3_SB(sb)->s_journal);
		if (err)
			return err;
	}

	return dquot_quota_on(sb, type, format_id, path);
}
