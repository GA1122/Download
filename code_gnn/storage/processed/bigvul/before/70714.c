evutil_accept4_(evutil_socket_t sockfd, struct sockaddr *addr,
    ev_socklen_t *addrlen, int flags)
{
	evutil_socket_t result;
#if defined(EVENT__HAVE_ACCEPT4) && defined(SOCK_CLOEXEC) && defined(SOCK_NONBLOCK)
	result = accept4(sockfd, addr, addrlen, flags);
	if (result >= 0 || (errno != EINVAL && errno != ENOSYS)) {
		 
		return result;
	}
#endif
	result = accept(sockfd, addr, addrlen);
	if (result < 0)
		return result;

	if (flags & EVUTIL_SOCK_CLOEXEC) {
		if (evutil_fast_socket_closeonexec(result) < 0) {
			evutil_closesocket(result);
			return -1;
		}
	}
	if (flags & EVUTIL_SOCK_NONBLOCK) {
		if (evutil_fast_socket_nonblocking(result) < 0) {
			evutil_closesocket(result);
			return -1;
		}
	}
	return result;
}
