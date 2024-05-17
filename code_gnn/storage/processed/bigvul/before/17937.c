static int php_stream_memory_seek(php_stream *stream, off_t offset, int whence, off_t *newoffs TSRMLS_DC)
{
	php_stream_memory_data *ms = (php_stream_memory_data*)stream->abstract;
	assert(ms != NULL);

	switch(whence) {
		case SEEK_CUR:
			if (offset < 0) {
				if (ms->fpos < (size_t)(-offset)) {
					ms->fpos = 0;
					*newoffs = -1;
					return -1;
				} else {
					ms->fpos = ms->fpos + offset;
					*newoffs = ms->fpos;
					stream->eof = 0;
					return 0;
				}
			} else {
				if (ms->fpos + (size_t)(offset) > ms->fsize) {
					ms->fpos = ms->fsize;
					*newoffs = -1;
					return -1;
				} else {
					ms->fpos = ms->fpos + offset;
					*newoffs = ms->fpos;
					stream->eof = 0;
					return 0;
				}
			}
		case SEEK_SET:
			if (ms->fsize < (size_t)(offset)) {
				ms->fpos = ms->fsize;
				*newoffs = -1;
				return -1;
			} else {
				ms->fpos = offset;
				*newoffs = ms->fpos;
				stream->eof = 0;
				return 0;
			}
		case SEEK_END:
			if (offset > 0) {
				ms->fpos = ms->fsize;
				*newoffs = -1;
				return -1;
			} else if (ms->fsize < (size_t)(-offset)) {
				ms->fpos = 0;
				*newoffs = -1;
				return -1;
			} else {
				ms->fpos = ms->fsize + offset;
				*newoffs = ms->fpos;
				stream->eof = 0;
				return 0;
			}
		default:
			*newoffs = ms->fpos;
			return -1;
	}
}
