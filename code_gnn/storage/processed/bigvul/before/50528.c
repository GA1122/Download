static void put_event(struct perf_event *event)
{
	struct perf_event_context *ctx = event->ctx;

	if (!atomic_long_dec_and_test(&event->refcount))
		return;

	if (!is_kernel_event(event))
		perf_remove_from_owner(event);

	WARN_ON_ONCE(ctx->parent_ctx);
	 
	mutex_lock_nested(&ctx->mutex, SINGLE_DEPTH_NESTING);
	perf_remove_from_context(event, true);
	mutex_unlock(&ctx->mutex);

	_free_event(event);
}
