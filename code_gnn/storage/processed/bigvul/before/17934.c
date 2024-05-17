static int php_stream_memory_close(php_stream *stream, int close_handle TSRMLS_DC)
{
	php_stream_memory_data *ms = (php_stream_memory_data*)stream->abstract;
	assert(ms != NULL);

	if (ms->data && close_handle && ms->mode != TEMP_STREAM_READONLY) {
		efree(ms->data);
	}
	efree(ms);
	return 0;
}
