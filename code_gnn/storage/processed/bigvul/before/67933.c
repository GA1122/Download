jas_stream_t *jas_stream_tmpfile()
{
	jas_stream_t *stream;
	jas_stream_fileobj_t *obj;

	JAS_DBGLOG(100, ("jas_stream_tmpfile()\n"));

	if (!(stream = jas_stream_create())) {
		return 0;
	}

	 
	stream->openmode_ = JAS_STREAM_READ | JAS_STREAM_WRITE | JAS_STREAM_BINARY;

	 
	if (!(obj = jas_malloc(sizeof(jas_stream_fileobj_t)))) {
		jas_stream_destroy(stream);
		return 0;
	}
	obj->fd = -1;
	obj->flags = 0;
	obj->pathname[0] = '\0';
	stream->obj_ = obj;

	 
	tmpnam(obj->pathname);

	 
	if ((obj->fd = open(obj->pathname, O_CREAT | O_EXCL | O_RDWR | O_TRUNC | O_BINARY,
	  JAS_STREAM_PERMS)) < 0) {
		jas_stream_destroy(stream);
		return 0;
	}

	 
	 
	if (unlink(obj->pathname)) {
		 
		obj->flags |= JAS_STREAM_FILEOBJ_DELONCLOSE;
	}

	 
	jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

	stream->ops_ = &jas_stream_fileops;

	return stream;
}
