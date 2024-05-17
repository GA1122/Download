evutil_make_listen_socket_reuseable(evutil_socket_t sock)
{
#if defined(SO_REUSEADDR) && !defined(_WIN32)
	int one = 1;
	 
	return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*) &one,
	    (ev_socklen_t)sizeof(one));
#else
	return 0;
#endif
}
