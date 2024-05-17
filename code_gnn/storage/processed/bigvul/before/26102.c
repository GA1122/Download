int perf_event_release_kernel(struct perf_event *event)
{
	struct perf_event_context *ctx = event->ctx;

	WARN_ON_ONCE(ctx->parent_ctx);
	 
	mutex_lock_nested(&ctx->mutex, SINGLE_DEPTH_NESTING);
	raw_spin_lock_irq(&ctx->lock);
	perf_group_detach(event);
	raw_spin_unlock_irq(&ctx->lock);
	perf_remove_from_context(event);
	mutex_unlock(&ctx->mutex);

	free_event(event);

	return 0;
}
