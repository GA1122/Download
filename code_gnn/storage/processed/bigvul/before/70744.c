evutil_make_listen_socket_reuseable_port(evutil_socket_t sock)
{
#if defined __linux__ && defined(SO_REUSEPORT)
	int one = 1;
	 
	return setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (void*) &one,
	    (ev_socklen_t)sizeof(one));
#else
	return 0;
#endif
}
