PHP_FUNCTION(stream_socket_server)
{
	char *host;
	int host_len;
	zval *zerrno = NULL, *zerrstr = NULL, *zcontext = NULL;
	php_stream *stream = NULL;
	int err = 0;
	long flags = STREAM_XPORT_BIND | STREAM_XPORT_LISTEN;
	char *errstr = NULL;
	php_stream_context *context = NULL;

	RETVAL_FALSE;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|zzlr", &host, &host_len, &zerrno, &zerrstr, &flags, &zcontext) == FAILURE) {
		RETURN_FALSE;
	}

	context = php_stream_context_from_zval(zcontext, flags & PHP_FILE_NO_DEFAULT_CONTEXT);

	if (context) {
		zend_list_addref(context->rsrc_id);
	}

	if (zerrno)	{
		zval_dtor(zerrno);
		ZVAL_LONG(zerrno, 0);
	}
	if (zerrstr) {
		zval_dtor(zerrstr);
		ZVAL_STRING(zerrstr, "", 1);
	}

	stream = php_stream_xport_create(host, host_len, REPORT_ERRORS,
			STREAM_XPORT_SERVER | flags,
			NULL, NULL, context, &errstr, &err);

	if (stream == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "unable to connect to %s (%s)", host, errstr == NULL ? "Unknown error" : errstr);
	}

	if (stream == NULL)	{
		if (zerrno) {
			zval_dtor(zerrno);
			ZVAL_LONG(zerrno, err);
		}
		if (zerrstr && errstr) {
			 
			zval_dtor(zerrstr);
			ZVAL_STRING(zerrstr, errstr, 0);
		} else if (errstr) {
			efree(errstr);
		}
		RETURN_FALSE;
	}

	if (errstr) {
		efree(errstr);
	}

	php_stream_to_zval(stream, return_value);
}
