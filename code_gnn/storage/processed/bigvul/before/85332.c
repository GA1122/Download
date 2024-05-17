static unsigned int __get_next_segno(struct f2fs_sb_info *sbi, int type)
{
	 
	if (sbi->segs_per_sec != 1)
		return CURSEG_I(sbi, type)->segno;

	if (type == CURSEG_HOT_DATA || IS_NODESEG(type))
		return 0;

	if (SIT_I(sbi)->last_victim[ALLOC_NEXT])
		return SIT_I(sbi)->last_victim[ALLOC_NEXT];
	return CURSEG_I(sbi, type)->segno;
}
