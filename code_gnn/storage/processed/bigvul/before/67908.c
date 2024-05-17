static void jas_stream_destroy(jas_stream_t *stream)
{
	JAS_DBGLOG(100, ("jas_stream_destroy(%p)\n", stream));

	 
	if ((stream->bufmode_ & JAS_STREAM_FREEBUF) && stream->bufbase_) {
		JAS_DBGLOG(100, ("jas_stream_destroy freeing buffer %p\n",
		  stream->bufbase_));
		jas_free(stream->bufbase_);
		stream->bufbase_ = 0;
	}
	jas_free(stream);
}