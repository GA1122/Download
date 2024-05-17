jas_stream_t *jas_stream_fdopen(int fd, const char *mode)
{
	jas_stream_t *stream;
	jas_stream_fileobj_t *obj;

	JAS_DBGLOG(100, ("jas_stream_fdopen(%d, \"%s\")\n", fd, mode));

	 
	if (!(stream = jas_stream_create())) {
		return 0;
	}

	 
	stream->openmode_ = jas_strtoopenmode(mode);

#if defined(WIN32)
	 
	 
	 
	if (stream->openmode_ & JAS_STREAM_BINARY) {
		setmode(fd, O_BINARY);
	}
#endif

	 
	if (!(obj = jas_malloc(sizeof(jas_stream_fileobj_t)))) {
		jas_stream_destroy(stream);
		return 0;
	}
	obj->fd = fd;
	obj->flags = 0;
	obj->pathname[0] = '\0';
	stream->obj_ = (void *) obj;

	 
	obj->flags |= JAS_STREAM_FILEOBJ_NOCLOSE;

	 
	jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

	 
	stream->ops_ = &jas_stream_fileops;

	return stream;
}
