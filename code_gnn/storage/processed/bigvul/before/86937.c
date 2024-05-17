static void free_utc(struct user_ta_ctx *utc)
{
	struct user_ta_elf *elf;

	tee_pager_rem_uta_areas(utc);
	TAILQ_FOREACH(elf, &utc->elfs, link)
		release_ta_memory_by_mobj(elf->mobj_code);
	release_ta_memory_by_mobj(utc->mobj_stack);
	release_ta_memory_by_mobj(utc->mobj_exidx);

	 
	while (!TAILQ_EMPTY(&utc->open_sessions)) {
		tee_ta_close_session(TAILQ_FIRST(&utc->open_sessions),
				     &utc->open_sessions, KERN_IDENTITY);
	}

	vm_info_final(utc);
	mobj_free(utc->mobj_stack);
	mobj_free(utc->mobj_exidx);
	free_elfs(&utc->elfs);

	 
	tee_svc_cryp_free_states(utc);
	 
	tee_obj_close_all(utc);
	 
	tee_svc_storage_close_all_enum(utc);
	free(utc);
}
