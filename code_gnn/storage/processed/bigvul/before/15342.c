static PHP_FUNCTION(ob_gzhandler)
{
	char *in_str;
	int in_len;
	long flags = 0;
	php_output_context ctx = {0};
	int encoding, rv;

	 

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &in_str, &in_len, &flags)) {
		RETURN_FALSE;
	}

	if (!(encoding = php_zlib_output_encoding(TSRMLS_C))) {
		RETURN_FALSE;
	}

	if (flags & PHP_OUTPUT_HANDLER_START) {
		switch (encoding) {
			case PHP_ZLIB_ENCODING_GZIP:
				sapi_add_header_ex(ZEND_STRL("Content-Encoding: gzip"), 1, 1 TSRMLS_CC);
				break;
			case PHP_ZLIB_ENCODING_DEFLATE:
				sapi_add_header_ex(ZEND_STRL("Content-Encoding: deflate"), 1, 1 TSRMLS_CC);
				break;
		}
		sapi_add_header_ex(ZEND_STRL("Vary: Accept-Encoding"), 1, 0 TSRMLS_CC);
	}

	if (!ZLIBG(ob_gzhandler)) {
		ZLIBG(ob_gzhandler) = php_zlib_output_handler_context_init(TSRMLS_C);
	}

	TSRMLS_SET_CTX(ctx.tsrm_ls);
	ctx.op = flags;
	ctx.in.data = in_str;
	ctx.in.used = in_len;

	rv = php_zlib_output_handler_ex(ZLIBG(ob_gzhandler), &ctx);

	if (SUCCESS != rv) {
		if (ctx.out.data && ctx.out.free) {
			efree(ctx.out.data);
		}
		php_zlib_cleanup_ob_gzhandler_mess(TSRMLS_C);
		RETURN_FALSE;
	}

	if (ctx.out.data) {
		RETVAL_STRINGL(ctx.out.data, ctx.out.used, 1);
		if (ctx.out.free) {
			efree(ctx.out.data);
		}
	} else {
		RETVAL_EMPTY_STRING();
	}
}
