static int ext3_write_info(struct super_block *sb, int type)
{
	int ret, err;
	handle_t *handle;

	 
	handle = ext3_journal_start(sb->s_root->d_inode, 2);
	if (IS_ERR(handle))
		return PTR_ERR(handle);
	ret = dquot_commit_info(sb, type);
	err = ext3_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}