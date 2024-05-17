static void perf_event_context_sched_out(struct task_struct *task, int ctxn,
					 struct task_struct *next)
{
	struct perf_event_context *ctx = task->perf_event_ctxp[ctxn];
	struct perf_event_context *next_ctx;
	struct perf_event_context *parent;
	struct perf_cpu_context *cpuctx;
	int do_switch = 1;

	if (likely(!ctx))
		return;

	cpuctx = __get_cpu_context(ctx);
	if (!cpuctx->task_ctx)
		return;

	rcu_read_lock();
	parent = rcu_dereference(ctx->parent_ctx);
	next_ctx = next->perf_event_ctxp[ctxn];
	if (parent && next_ctx &&
	    rcu_dereference(next_ctx->parent_ctx) == parent) {
		 
		raw_spin_lock(&ctx->lock);
		raw_spin_lock_nested(&next_ctx->lock, SINGLE_DEPTH_NESTING);
		if (context_equiv(ctx, next_ctx)) {
			 
			task->perf_event_ctxp[ctxn] = next_ctx;
			next->perf_event_ctxp[ctxn] = ctx;
			ctx->task = next;
			next_ctx->task = task;
			do_switch = 0;

			perf_event_sync_stat(ctx, next_ctx);
		}
		raw_spin_unlock(&next_ctx->lock);
		raw_spin_unlock(&ctx->lock);
	}
	rcu_read_unlock();

	if (do_switch) {
		raw_spin_lock(&ctx->lock);
		ctx_sched_out(ctx, cpuctx, EVENT_ALL);
		cpuctx->task_ctx = NULL;
		raw_spin_unlock(&ctx->lock);
	}
}