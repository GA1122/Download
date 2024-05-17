void tee_mmu_set_ctx(struct tee_ta_ctx *ctx)
{
	struct thread_specific_data *tsd = thread_get_tsd();

	core_mmu_set_user_map(NULL);
	 
	pgt_free(&tsd->pgt_cache, tsd->ctx && is_user_ta_ctx(tsd->ctx));

	if (ctx && is_user_ta_ctx(ctx)) {
		struct core_mmu_user_map map;
		struct user_ta_ctx *utc = to_user_ta_ctx(ctx);

		core_mmu_create_user_map(utc, &map);
		core_mmu_set_user_map(&map);
		tee_pager_assign_uta_tables(utc);
	}
	tsd->ctx = ctx;
}
