static inline void map_to_unicode(unsigned code, const enc_to_uni *table, unsigned *res)
{
	 
	*res = table->inner[ENT_ENC_TO_UNI_STAGE1(code)]->uni_cp[ENT_ENC_TO_UNI_STAGE2(code)];
}
