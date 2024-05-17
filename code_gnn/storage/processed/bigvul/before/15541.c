mux_client_write_packet(int fd, Buffer *m)
{
	Buffer queue;
	u_int have, need;
	int oerrno, len;
	u_char *ptr;
	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLOUT;
	buffer_init(&queue);
	buffer_put_string(&queue, buffer_ptr(m), buffer_len(m));

	need = buffer_len(&queue);
	ptr = buffer_ptr(&queue);

	for (have = 0; have < need; ) {
		if (muxclient_terminate) {
			buffer_free(&queue);
			errno = EINTR;
			return -1;
		}
		len = write(fd, ptr + have, need - have);
		if (len < 0) {
			switch (errno) {
#if defined(EWOULDBLOCK) && (EWOULDBLOCK != EAGAIN)
			case EWOULDBLOCK:
#endif
			case EAGAIN:
				(void)poll(&pfd, 1, -1);
				 
			case EINTR:
				continue;
			default:
				oerrno = errno;
				buffer_free(&queue);
				errno = oerrno;
				return -1;
			}
		}
		if (len == 0) {
			buffer_free(&queue);
			errno = EPIPE;
			return -1;
		}
		have += (u_int)len;
	}
	buffer_free(&queue);
	return 0;
}
