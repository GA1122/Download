int jas_stream_close(jas_stream_t *stream)
{
	JAS_DBGLOG(100, ("jas_stream_close(%p)\n", stream));

	 
	jas_stream_flush(stream);

	 
	(*stream->ops_->close_)(stream->obj_);

	jas_stream_destroy(stream);

	return 0;
}