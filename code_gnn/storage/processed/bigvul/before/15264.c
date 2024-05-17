PHP_FUNCTION(stream_context_set_option)
{
	zval *options = NULL, *zcontext = NULL, *zvalue = NULL;
	php_stream_context *context;
	char *wrappername, *optionname;
	int wrapperlen, optionlen;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC,
				"rssz", &zcontext, &wrappername, &wrapperlen,
				&optionname, &optionlen, &zvalue) == FAILURE) {
		if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC,
					"ra", &zcontext, &options) == FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "called with wrong number or type of parameters; please RTM");
			RETURN_FALSE;
		}
	}

	 
	context = decode_context_param(zcontext TSRMLS_CC);
	if (!context) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid stream/context parameter");
		RETURN_FALSE;
	}

	if (options) {
		 
		RETVAL_BOOL(parse_context_options(context, options TSRMLS_CC) == SUCCESS);
	} else {
		php_stream_context_set_option(context, wrappername, optionname, zvalue);
		RETVAL_TRUE;
	}
}
