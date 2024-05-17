int jas_stream_flushbuf(jas_stream_t *stream, int c)
{
	int len;
	int n;

	 
	if ((stream->flags_ & (JAS_STREAM_ERRMASK)) != 0) {
		return EOF;
	}

	 
	if ((stream->openmode_ & (JAS_STREAM_WRITE | JAS_STREAM_APPEND)) == 0) {
		return EOF;
	}

	 
	assert(!(stream->bufmode_ & JAS_STREAM_RDBUF));

	 
	len = stream->ptr_ - stream->bufstart_;
	if (len > 0) {
		n = (*stream->ops_->write_)(stream->obj_, (char *)
		  stream->bufstart_, len);
		if (n != len) {
			stream->flags_ |= JAS_STREAM_ERR;
			return EOF;
		}
	}
	stream->cnt_ = stream->bufsize_;
	stream->ptr_ = stream->bufstart_;

	stream->bufmode_ |= JAS_STREAM_WRBUF;

	if (c != EOF) {
		assert(stream->cnt_ > 0);
		return jas_stream_putc2(stream, c);
	}

	return 0;
}
