void exit_aio(struct mm_struct *mm)
{
	struct kioctx_table *table = rcu_dereference_raw(mm->ioctx_table);
	int i;

	if (!table)
		return;

	for (i = 0; i < table->nr; ++i) {
		struct kioctx *ctx = table->table[i];
		struct completion requests_done =
			COMPLETION_INITIALIZER_ONSTACK(requests_done);

		if (!ctx)
			continue;
		 
		ctx->mmap_size = 0;
		kill_ioctx(mm, ctx, &requests_done);

		 
		wait_for_completion(&requests_done);
	}

	RCU_INIT_POINTER(mm->ioctx_table, NULL);
	kfree(table);
}
