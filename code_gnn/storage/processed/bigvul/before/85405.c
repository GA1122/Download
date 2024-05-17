void invalidate_blocks(struct f2fs_sb_info *sbi, block_t addr)
{
	unsigned int segno = GET_SEGNO(sbi, addr);
	struct sit_info *sit_i = SIT_I(sbi);

	f2fs_bug_on(sbi, addr == NULL_ADDR);
	if (addr == NEW_ADDR)
		return;

	 
	mutex_lock(&sit_i->sentry_lock);

	update_sit_entry(sbi, addr, -1);

	 
	locate_dirty_segment(sbi, segno);

	mutex_unlock(&sit_i->sentry_lock);
}
