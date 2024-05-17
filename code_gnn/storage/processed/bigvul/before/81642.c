consume_socket(struct mg_context *ctx, struct socket *sp, int thread_index)
{
#define QUEUE_SIZE(ctx) ((int)(ARRAY_SIZE(ctx->queue)))

	(void)thread_index;

	(void)pthread_mutex_lock(&ctx->thread_mutex);
	DEBUG_TRACE("%s", "going idle");

	 
	while ((ctx->sq_head == ctx->sq_tail) && (ctx->stop_flag == 0)) {
		pthread_cond_wait(&ctx->sq_full, &ctx->thread_mutex);
	}

	 
	if (ctx->sq_head > ctx->sq_tail) {
		 
		*sp = ctx->queue[ctx->sq_tail % QUEUE_SIZE(ctx)];
		ctx->sq_tail++;

		DEBUG_TRACE("grabbed socket %d, going busy", sp ? sp->sock : -1);

		 
		while (ctx->sq_tail > QUEUE_SIZE(ctx)) {
			ctx->sq_tail -= QUEUE_SIZE(ctx);
			ctx->sq_head -= QUEUE_SIZE(ctx);
		}
	}

	(void)pthread_cond_signal(&ctx->sq_empty);
	(void)pthread_mutex_unlock(&ctx->thread_mutex);

	return !ctx->stop_flag;
#undef QUEUE_SIZE
}
