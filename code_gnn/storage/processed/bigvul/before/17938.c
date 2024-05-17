static size_t php_stream_memory_write(php_stream *stream, const char *buf, size_t count TSRMLS_DC)
{
	php_stream_memory_data *ms = (php_stream_memory_data*)stream->abstract;
	assert(ms != NULL);

	if (ms->mode & TEMP_STREAM_READONLY) {
		return 0;
	}
	if (ms->fpos + count > ms->fsize) {
		char *tmp;

		if (!ms->data) {
			tmp = emalloc(ms->fpos + count);
		} else {
			tmp = erealloc(ms->data, ms->fpos + count);
		}
		if (!tmp) {
			count = ms->fsize - ms->fpos + 1;
		} else {
			ms->data = tmp;
			ms->fsize = ms->fpos + count;
		}
	}
	if (!ms->data)
		count = 0;
	if (count) {
		assert(buf!= NULL);
		memcpy(ms->data+ms->fpos, (char*)buf, count);
		ms->fpos += count;
	}
	return count;
}
