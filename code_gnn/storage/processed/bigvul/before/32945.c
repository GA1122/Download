static int ext3_release_dquot(struct dquot *dquot)
{
	int ret, err;
	handle_t *handle;

	handle = ext3_journal_start(dquot_to_inode(dquot),
					EXT3_QUOTA_DEL_BLOCKS(dquot->dq_sb));
	if (IS_ERR(handle)) {
		 
		dquot_release(dquot);
		return PTR_ERR(handle);
	}
	ret = dquot_release(dquot);
	err = ext3_journal_stop(handle);
	if (!ret)
		ret = err;
	return ret;
}