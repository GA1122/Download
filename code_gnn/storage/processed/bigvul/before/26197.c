static void rotate_ctx(struct perf_event_context *ctx)
{
	 
	if (!ctx->rotate_disable)
		list_rotate_left(&ctx->flexible_groups);
}
