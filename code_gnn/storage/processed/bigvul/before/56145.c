static void put_event(struct perf_event *event)
{
	struct perf_event_context *ctx;

	if (!atomic_long_dec_and_test(&event->refcount))
		return;

	if (!is_kernel_event(event))
		perf_remove_from_owner(event);

	 
	ctx = perf_event_ctx_lock_nested(event, SINGLE_DEPTH_NESTING);
	WARN_ON_ONCE(ctx->parent_ctx);
	perf_remove_from_context(event, true);
	perf_event_ctx_unlock(event, ctx);

	_free_event(event);
}
