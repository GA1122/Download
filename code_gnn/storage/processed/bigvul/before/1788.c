int FAST_FUNC start_bunzip(bunzip_data **bdp, int in_fd,
		const void *inbuf, int len)
{
	bunzip_data *bd;
	unsigned i;
	enum {
		BZh0 = ('B' << 24) + ('Z' << 16) + ('h' << 8) + '0',
		h0 = ('h' << 8) + '0',
	};

	 
	i = sizeof(bunzip_data);
	if (in_fd != -1) i += IOBUF_SIZE;

	 
	bd = *bdp = xzalloc(i);

	 
	bd->in_fd = in_fd;
	if (-1 == in_fd) {
		 
		bd->inbuf = (void*)inbuf;  
	} else {
		bd->inbuf = (uint8_t*)(bd + 1);
		memcpy(bd->inbuf, inbuf, len);
	}
	bd->inbufCount = len;

	 
	crc32_filltable(bd->crc32Table, 1);

	 
	i = setjmp(bd->jmpbuf);
	if (i) return i;

	 
	 
	 
	 
	 
	i = get_bits(bd, 16);
	if ((unsigned)(i - h0 - 1) >= 9) return RETVAL_NOT_BZIP_DATA;

	 
	 
	bd->dbufSize = 100000 * (i - h0);

	 
	bd->dbuf = malloc_or_warn(bd->dbufSize * sizeof(bd->dbuf[0]));
	if (!bd->dbuf) {
		free(bd);
		xfunc_die();
	}
	return RETVAL_OK;
}
