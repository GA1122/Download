void exit_aio(struct mm_struct *mm)
{
	struct kioctx *ctx;

	while (!hlist_empty(&mm->ioctx_list)) {
		ctx = hlist_entry(mm->ioctx_list.first, struct kioctx, list);
		hlist_del_rcu(&ctx->list);

		kill_ctx(ctx);

		if (1 != atomic_read(&ctx->users))
			printk(KERN_DEBUG
				"exit_aio:ioctx still alive: %d %d %d\n",
				atomic_read(&ctx->users), ctx->dead,
				ctx->reqs_active);
		 
		ctx->ring_info.mmap_size = 0;
		put_ioctx(ctx);
	}
}
