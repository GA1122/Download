PHPAPI char *_php_stream_memory_get_buffer(php_stream *stream, size_t *length STREAMS_DC TSRMLS_DC)
{
	php_stream_memory_data *ms = (php_stream_memory_data*)stream->abstract;

	assert(ms != NULL);
	assert(length != 0);

	*length = ms->fsize;
	return ms->data;
}
