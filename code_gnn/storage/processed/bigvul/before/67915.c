jas_stream_t *jas_stream_freopen(const char *path, const char *mode, FILE *fp)
{
	jas_stream_t *stream;
	int openflags;

	JAS_DBGLOG(100, ("jas_stream_freopen(\"%s\", \"%s\", %p)\n", path, mode,
	  fp));

	 
	path = 0;

	 
	if (!(stream = jas_stream_create())) {
		return 0;
	}

	 
	stream->openmode_ = jas_strtoopenmode(mode);

	 
	if ((stream->openmode_ & JAS_STREAM_READ) &&
	  (stream->openmode_ & JAS_STREAM_WRITE)) {
		openflags = O_RDWR;
	} else if (stream->openmode_ & JAS_STREAM_READ) {
		openflags = O_RDONLY;
	} else if (stream->openmode_ & JAS_STREAM_WRITE) {
		openflags = O_WRONLY;
	} else {
		openflags = 0;
	}
	if (stream->openmode_ & JAS_STREAM_APPEND) {
		openflags |= O_APPEND;
	}
	if (stream->openmode_ & JAS_STREAM_BINARY) {
		openflags |= O_BINARY;
	}
	if (stream->openmode_ & JAS_STREAM_CREATE) {
		openflags |= O_CREAT | O_TRUNC;
	}

	stream->obj_ = JAS_CAST(void *, fp);

	 
	stream->ops_ = &jas_stream_sfileops;

	 
	jas_stream_initbuf(stream, JAS_STREAM_FULLBUF, 0, 0);

	return stream;
}
