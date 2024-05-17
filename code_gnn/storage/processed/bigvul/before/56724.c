static void kill_ioctx(struct mm_struct *mm, struct kioctx *ctx,
		struct completion *requests_done)
{
	if (!atomic_xchg(&ctx->dead, 1)) {
		struct kioctx_table *table;

		spin_lock(&mm->ioctx_lock);
		rcu_read_lock();
		table = rcu_dereference(mm->ioctx_table);

		WARN_ON(ctx != table->table[ctx->id]);
		table->table[ctx->id] = NULL;
		rcu_read_unlock();
		spin_unlock(&mm->ioctx_lock);

		 
		wake_up_all(&ctx->wait);

		 
		aio_nr_sub(ctx->max_reqs);

		if (ctx->mmap_size)
			vm_munmap(ctx->mmap_base, ctx->mmap_size);

		ctx->requests_done = requests_done;
		percpu_ref_kill(&ctx->users);
	} else {
		if (requests_done)
			complete(requests_done);
	}
}