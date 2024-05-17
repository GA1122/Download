static size_t php_stream_temp_write(php_stream *stream, const char *buf, size_t count TSRMLS_DC)
{
	php_stream_temp_data *ts = (php_stream_temp_data*)stream->abstract;
	assert(ts != NULL);

	if (!ts->innerstream) {
		return -1;
	}
	if (php_stream_is(ts->innerstream, PHP_STREAM_IS_MEMORY)) {
		size_t memsize;
		char *membuf = php_stream_memory_get_buffer(ts->innerstream, &memsize);

		if (memsize + count >= ts->smax) {
			php_stream *file = php_stream_fopen_tmpfile();
			if (file == NULL) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to create temporary file, Check permissions in temporary files directory.");
				return 0;
			}
			php_stream_write(file, membuf, memsize);
			php_stream_free_enclosed(ts->innerstream, PHP_STREAM_FREE_CLOSE);
			ts->innerstream = file;
			php_stream_encloses(stream, ts->innerstream);
		}
	}
	return php_stream_write(ts->innerstream, buf, count);
}
