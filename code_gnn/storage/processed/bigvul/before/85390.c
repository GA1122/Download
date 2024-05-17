void f2fs_wait_discard_bio(struct f2fs_sb_info *sbi, block_t blkaddr)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	struct discard_cmd *dc;
	bool need_wait = false;

	mutex_lock(&dcc->cmd_lock);
	dc = (struct discard_cmd *)__lookup_rb_tree(&dcc->root, NULL, blkaddr);
	if (dc) {
		if (dc->state == D_PREP) {
			__punch_discard_cmd(sbi, dc, blkaddr);
		} else {
			dc->ref++;
			need_wait = true;
		}
	}
	mutex_unlock(&dcc->cmd_lock);

	if (need_wait) {
		wait_for_completion_io(&dc->wait);
		mutex_lock(&dcc->cmd_lock);
		f2fs_bug_on(sbi, dc->state != D_DONE);
		dc->ref--;
		if (!dc->ref)
			__remove_discard_cmd(sbi, dc);
		mutex_unlock(&dcc->cmd_lock);
	}
}