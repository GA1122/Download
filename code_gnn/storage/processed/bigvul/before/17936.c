static size_t php_stream_memory_read(php_stream *stream, char *buf, size_t count TSRMLS_DC)
{
	php_stream_memory_data *ms = (php_stream_memory_data*)stream->abstract;
	assert(ms != NULL);

	if (ms->fpos == ms->fsize) {
		stream->eof = 1;
		count = 0;
	} else {
		if (ms->fpos + count >= ms->fsize) {
			count = ms->fsize - ms->fpos;
		}
		if (count) {
			assert(ms->data!= NULL);
			assert(buf!= NULL);
			memcpy(buf, ms->data+ms->fpos, count);
			ms->fpos += count;
		}
	}
	return count;
}
