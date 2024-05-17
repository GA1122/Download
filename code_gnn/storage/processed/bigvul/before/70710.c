apply_numeric_port_hack(int port, struct evutil_addrinfo **ai)
{
	 
	for ( ; *ai; ai = &(*ai)->ai_next) {
		struct sockaddr *sa = (*ai)->ai_addr;
		if (sa && sa->sa_family == AF_INET) {
			struct sockaddr_in *sin = (struct sockaddr_in*)sa;
			sin->sin_port = htons(port);
		} else if (sa && sa->sa_family == AF_INET6) {
			struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)sa;
			sin6->sin6_port = htons(port);
		} else {
			 
			struct evutil_addrinfo *victim = *ai;
			*ai = victim->ai_next;
			victim->ai_next = NULL;
			freeaddrinfo(victim);
		}
	}
}
