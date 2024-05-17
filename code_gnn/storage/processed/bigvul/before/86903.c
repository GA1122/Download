 static TEE_Result alloc_temp_sec_mem(size_t size, struct mobj **mobj,
				     uint8_t **va)
{
	 
#ifdef CFG_PAGED_USER_TA
	*mobj = mobj_seccpy_shm_alloc(size);
#else
	*mobj = mobj_mm_alloc(mobj_sec_ddr, size, &tee_mm_sec_ddr);
#endif
	if (!*mobj)
		return TEE_ERROR_GENERIC;

	*va = mobj_get_va(*mobj, 0);
	return TEE_SUCCESS;
}