static int get_ssr_segment(struct f2fs_sb_info *sbi, int type)
{
	struct curseg_info *curseg = CURSEG_I(sbi, type);
	const struct victim_selection *v_ops = DIRTY_I(sbi)->v_ops;
	unsigned segno = NULL_SEGNO;
	int i, cnt;
	bool reversed = false;

	 
	if (v_ops->get_victim(sbi, &segno, BG_GC, type, SSR)) {
		curseg->next_segno = segno;
		return 1;
	}

	 
	if (IS_NODESEG(type)) {
		if (type >= CURSEG_WARM_NODE) {
			reversed = true;
			i = CURSEG_COLD_NODE;
		} else {
			i = CURSEG_HOT_NODE;
		}
		cnt = NR_CURSEG_NODE_TYPE;
	} else {
		if (type >= CURSEG_WARM_DATA) {
			reversed = true;
			i = CURSEG_COLD_DATA;
		} else {
			i = CURSEG_HOT_DATA;
		}
		cnt = NR_CURSEG_DATA_TYPE;
	}

	for (; cnt-- > 0; reversed ? i-- : i++) {
		if (i == type)
			continue;
		if (v_ops->get_victim(sbi, &segno, BG_GC, i, SSR)) {
			curseg->next_segno = segno;
			return 1;
		}
	}
	return 0;
}