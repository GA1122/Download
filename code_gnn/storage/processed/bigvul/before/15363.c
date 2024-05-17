static void php_zlib_output_handler_context_dtor(void *opaq TSRMLS_DC)
{
	php_zlib_context *ctx = (php_zlib_context *) opaq;

	if (ctx) {
		if (ctx->buffer.data) {
			efree(ctx->buffer.data);
		}
		efree(ctx);
	}
}
