PHPAPI PHP_FUNCTION(fclose)
{
	zval *arg1;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &arg1) == FAILURE) {
		RETURN_FALSE;
	}

	PHP_STREAM_TO_ZVAL(stream, &arg1);

	if ((stream->flags & PHP_STREAM_FLAG_NO_FCLOSE) != 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d is not a valid stream resource", stream->rsrc_id);
		RETURN_FALSE;
	}

	if (!stream->is_persistent) {
		php_stream_close(stream);
	} else {
		php_stream_pclose(stream);
	}

	RETURN_TRUE;
}
