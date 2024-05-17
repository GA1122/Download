PHPAPI php_stream *_php_stream_temp_open(int mode, size_t max_memory_usage, char *buf, size_t length STREAMS_DC TSRMLS_DC)
{
	php_stream *stream;
	php_stream_temp_data *ts;
	off_t newoffs;

	if ((stream = php_stream_temp_create_rel(mode, max_memory_usage)) != NULL) {
		if (length) {
			assert(buf != NULL);
			php_stream_temp_write(stream, buf, length TSRMLS_CC);
			php_stream_temp_seek(stream, 0, SEEK_SET, &newoffs TSRMLS_CC);
		}
		ts = (php_stream_temp_data*)stream->abstract;
		assert(ts != NULL);
		ts->mode = mode;
	}
	return stream;
}
