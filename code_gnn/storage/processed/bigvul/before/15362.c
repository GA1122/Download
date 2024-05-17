static int php_zlib_output_handler(void **handler_context, php_output_context *output_context)
{
	php_zlib_context *ctx = *(php_zlib_context **) handler_context;
	PHP_OUTPUT_TSRMLS(output_context);

	if (!php_zlib_output_encoding(TSRMLS_C)) {
		 
		if ((output_context->op & PHP_OUTPUT_HANDLER_START)
		&&	(output_context->op != (PHP_OUTPUT_HANDLER_START|PHP_OUTPUT_HANDLER_CLEAN|PHP_OUTPUT_HANDLER_FINAL))
		) {
			sapi_add_header_ex(ZEND_STRL("Vary: Accept-Encoding"), 1, 0 TSRMLS_CC);
		}
		return FAILURE;
	}

	if (SUCCESS != php_zlib_output_handler_ex(ctx, output_context)) {
		return FAILURE;
	}

	if (!(output_context->op & PHP_OUTPUT_HANDLER_CLEAN)) {
		int flags;

		if (SUCCESS == php_output_handler_hook(PHP_OUTPUT_HANDLER_HOOK_GET_FLAGS, &flags TSRMLS_CC)) {
			 
			if (!(flags & PHP_OUTPUT_HANDLER_STARTED)) {
				if (SG(headers_sent) || !ZLIBG(output_compression)) {
					deflateEnd(&ctx->Z);
					return FAILURE;
				}
				switch (ZLIBG(compression_coding)) {
					case PHP_ZLIB_ENCODING_GZIP:
						sapi_add_header_ex(ZEND_STRL("Content-Encoding: gzip"), 1, 1 TSRMLS_CC);
						break;
					case PHP_ZLIB_ENCODING_DEFLATE:
						sapi_add_header_ex(ZEND_STRL("Content-Encoding: deflate"), 1, 1 TSRMLS_CC);
						break;
					default:
						deflateEnd(&ctx->Z);
						return FAILURE;
				}
				sapi_add_header_ex(ZEND_STRL("Vary: Accept-Encoding"), 1, 0 TSRMLS_CC);
				php_output_handler_hook(PHP_OUTPUT_HANDLER_HOOK_IMMUTABLE, NULL TSRMLS_CC);
			}
		}
	}

	return SUCCESS;
}
