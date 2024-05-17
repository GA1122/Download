int jas_stream_fillbuf(jas_stream_t *stream, int getflag)
{
	int c;

	 
	if ((stream->flags_ & (JAS_STREAM_ERRMASK)) != 0) {
		return EOF;
	}

	 
	if ((stream->openmode_ & JAS_STREAM_READ) == 0) {
		return EOF;
	}

	 
	assert((stream->bufmode_ & JAS_STREAM_WRBUF) == 0);

	assert(stream->ptr_ - stream->bufstart_ <= stream->bufsize_);

	 
	stream->bufmode_ |= JAS_STREAM_RDBUF;

	 
	stream->ptr_ = stream->bufstart_;
	if ((stream->cnt_ = (*stream->ops_->read_)(stream->obj_,
	  (char *) stream->bufstart_, stream->bufsize_)) <= 0) {
		if (stream->cnt_ < 0) {
			stream->flags_ |= JAS_STREAM_ERR;
		} else {
			stream->flags_ |= JAS_STREAM_EOF;
		}
		stream->cnt_ = 0;
		return EOF;
	}

	assert(stream->cnt_ > 0);
	 
	c = (getflag) ? jas_stream_getc2(stream) : (*stream->ptr_);

	return c;
}
