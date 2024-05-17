free_context(struct mg_context *ctx)
{
	int i;
	struct mg_handler_info *tmp_rh;

	if (ctx == NULL) {
		return;
	}

	if (ctx->callbacks.exit_context) {
		ctx->callbacks.exit_context(ctx);
	}

	 
	(void)pthread_mutex_destroy(&ctx->thread_mutex);
#if defined(ALTERNATIVE_QUEUE)
	mg_free(ctx->client_socks);
	for (i = 0; (unsigned)i < ctx->cfg_worker_threads; i++) {
		event_destroy(ctx->client_wait_events[i]);
	}
	mg_free(ctx->client_wait_events);
#else
	(void)pthread_cond_destroy(&ctx->sq_empty);
	(void)pthread_cond_destroy(&ctx->sq_full);
#endif

	 
	(void)pthread_mutex_destroy(&ctx->nonce_mutex);

#if defined(USE_TIMERS)
	timers_exit(ctx);
#endif

	 
	for (i = 0; i < NUM_OPTIONS; i++) {
		if (ctx->dd.config[i] != NULL) {
#if defined(_MSC_VER)
#pragma warning(suppress : 6001)
#endif
			mg_free(ctx->dd.config[i]);
		}
	}

	 
	while (ctx->dd.handlers) {
		tmp_rh = ctx->dd.handlers;
		ctx->dd.handlers = tmp_rh->next;
		if (tmp_rh->handler_type == REQUEST_HANDLER) {
			pthread_cond_destroy(&tmp_rh->refcount_cond);
			pthread_mutex_destroy(&tmp_rh->refcount_mutex);
		}
		mg_free(tmp_rh->uri);
		mg_free(tmp_rh);
	}

#if !defined(NO_SSL)
	 
	if (ctx->dd.ssl_ctx != NULL) {
		void *ssl_ctx = (void *)ctx->dd.ssl_ctx;
		int callback_ret =
		    (ctx->callbacks.external_ssl_ctx == NULL)
		        ? 0
		        : (ctx->callbacks.external_ssl_ctx(&ssl_ctx, ctx->user_data));

		if (callback_ret == 0) {
			SSL_CTX_free(ctx->dd.ssl_ctx);
		}
		 
	}
#endif  

	 
	if (ctx->worker_threadids != NULL) {
		mg_free(ctx->worker_threadids);
	}

	 
	if (ctx->worker_connections != NULL) {
		mg_free(ctx->worker_connections);
	}

	 
	mg_free(ctx->systemName);

	 
	mg_free(ctx);
}
