static void jas_stream_destroy(jas_stream_t *stream)
{
	 
	if ((stream->bufmode_ & JAS_STREAM_FREEBUF) && stream->bufbase_) {
		jas_free(stream->bufbase_);
		stream->bufbase_ = 0;
	}
	jas_free(stream);
}
