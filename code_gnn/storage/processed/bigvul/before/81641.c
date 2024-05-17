consume_socket(struct mg_context *ctx, struct socket *sp, int thread_index)
{
	DEBUG_TRACE("%s", "going idle");
	ctx->client_socks[thread_index].in_use = 0;
	event_wait(ctx->client_wait_events[thread_index]);
	*sp = ctx->client_socks[thread_index];
	DEBUG_TRACE("grabbed socket %d, going busy", sp ? sp->sock : -1);

	return !ctx->stop_flag;
}
