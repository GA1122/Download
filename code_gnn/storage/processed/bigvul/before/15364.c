static php_zlib_context *php_zlib_output_handler_context_init(TSRMLS_D)
{
	php_zlib_context *ctx = (php_zlib_context *) ecalloc(1, sizeof(php_zlib_context));
	ctx->Z.zalloc = php_zlib_alloc;
	ctx->Z.zfree = php_zlib_free;
	return ctx;
}
