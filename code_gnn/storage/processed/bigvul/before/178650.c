 static int php_stream_temp_cast(php_stream *stream, int castas, void **ret TSRMLS_DC)
 {
 	php_stream_temp_data *ts = (php_stream_temp_data*)stream->abstract;
 	php_stream *file;
 	size_t memsize;
 	char *membuf;
 	off_t pos;
 
 	assert(ts != NULL);
 
 	if (!ts->innerstream) {
 		return FAILURE;
 	}
 	if (php_stream_is(ts->innerstream, PHP_STREAM_IS_STDIO)) {
 		return php_stream_cast(ts->innerstream, castas, ret, 0);
 	}
 
 	 
 
 	if (ret == NULL && castas == PHP_STREAM_AS_STDIO) {
 		return SUCCESS;
 	}
 
 	 
 	if (ret == NULL) {
 		return FAILURE;
 	}
 
 	 
 	membuf = php_stream_memory_get_buffer(ts->innerstream, &memsize);
         file = php_stream_fopen_tmpfile();
         php_stream_write(file, membuf, memsize);
         pos = php_stream_tell(ts->innerstream);
// 
         php_stream_free_enclosed(ts->innerstream, PHP_STREAM_FREE_CLOSE);
         ts->innerstream = file;
         php_stream_encloses(stream, ts->innerstream);
 	php_stream_seek(ts->innerstream, pos, SEEK_SET);
 
 	return php_stream_cast(ts->innerstream, castas, ret, 1);
 }