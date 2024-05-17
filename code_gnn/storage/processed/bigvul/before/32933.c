static int ext3_mark_dquot_dirty(struct dquot *dquot)
{
	 
	if (EXT3_SB(dquot->dq_sb)->s_qf_names[USRQUOTA] ||
	    EXT3_SB(dquot->dq_sb)->s_qf_names[GRPQUOTA]) {
		dquot_mark_dquot_dirty(dquot);
		return ext3_write_dquot(dquot);
	} else {
		return dquot_mark_dquot_dirty(dquot);
	}
}