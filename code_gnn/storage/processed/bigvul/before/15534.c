mux_client_read(int fd, Buffer *b, u_int need)
{
	u_int have;
	ssize_t len;
	u_char *p;
	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	p = buffer_append_space(b, need);
	for (have = 0; have < need; ) {
		if (muxclient_terminate) {
			errno = EINTR;
			return -1;
		}
		len = read(fd, p + have, need - have);
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
				return -1;
			}
		}
		if (len == 0) {
			errno = EPIPE;
			return -1;
		}
		have += (u_int)len;
	}
	return 0;
}
