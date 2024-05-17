static int ext4_write_info(struct super_block *sb, int type)
{
	int ret, err;
	handle_t *handle;

	 
	handle = ext4_journal_start(d_inode(sb->s_root), EXT4_HT_QUOTA, 2);
	if (IS_ERR(handle))
		return PTR_ERR(handle);
	ret = dquot_commit_info(sb, type);
	err = ext4_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}