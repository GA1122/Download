static int php_stream_temp_flush(php_stream *stream TSRMLS_DC)
{
	php_stream_temp_data *ts = (php_stream_temp_data*)stream->abstract;
	assert(ts != NULL);

	return ts->innerstream ? php_stream_flush(ts->innerstream) : -1;
}
