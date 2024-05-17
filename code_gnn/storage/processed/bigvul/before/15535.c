mux_client_read_packet(int fd, Buffer *m)
{
	Buffer queue;
	u_int need, have;
	const u_char *ptr;
	int oerrno;

	buffer_init(&queue);
	if (mux_client_read(fd, &queue, 4) != 0) {
		if ((oerrno = errno) == EPIPE)
			debug3("%s: read header failed: %s", __func__,
			    strerror(errno));
		buffer_free(&queue);
		errno = oerrno;
		return -1;
	}
	need = get_u32(buffer_ptr(&queue));
	if (mux_client_read(fd, &queue, need) != 0) {
		oerrno = errno;
		debug3("%s: read body failed: %s", __func__, strerror(errno));
		buffer_free(&queue);
		errno = oerrno;
		return -1;
	}
	ptr = buffer_get_string_ptr(&queue, &have);
	buffer_append(m, ptr, have);
	buffer_free(&queue);
	return 0;
}
