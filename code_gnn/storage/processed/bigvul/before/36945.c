miniwget3(const char * host,
          unsigned short port, const char * path,
          int * size, char * addr_str, int addr_str_len,
          const char * httpversion, unsigned int scope_id)
{
	char buf[2048];
    int s;
	int n;
	int len;
	int sent;
	void * content;

	*size = 0;
	s = connecthostport(host, port, scope_id);
	if(s < 0)
		return NULL;

	 
	if(addr_str)
	{
		struct sockaddr_storage saddr;
		socklen_t saddrlen;

		saddrlen = sizeof(saddr);
		if(getsockname(s, (struct sockaddr *)&saddr, &saddrlen) < 0)
		{
			perror("getsockname");
		}
		else
		{
#if defined(__amigaos__) && !defined(__amigaos4__)
	 
 
			 
			strncpy(addr_str, inet_ntoa(((struct sockaddr_in *)&saddr)->sin_addr), addr_str_len);
#else
#if 0
			if(saddr.sa_family == AF_INET6) {
				inet_ntop(AF_INET6,
				          &(((struct sockaddr_in6 *)&saddr)->sin6_addr),
				          addr_str, addr_str_len);
			} else {
				inet_ntop(AF_INET,
				          &(((struct sockaddr_in *)&saddr)->sin_addr),
				          addr_str, addr_str_len);
			}
#endif
			 
			n = getnameinfo((const struct sockaddr *)&saddr, saddrlen,
			                addr_str, addr_str_len,
			                NULL, 0,
			                NI_NUMERICHOST | NI_NUMERICSERV);
			if(n != 0) {
#ifdef _WIN32
				fprintf(stderr, "getnameinfo() failed : %d\n", n);
#else
				fprintf(stderr, "getnameinfo() failed : %s\n", gai_strerror(n));
#endif
			}
#endif
		}
#ifdef DEBUG
		printf("address miniwget : %s\n", addr_str);
#endif
	}

	len = snprintf(buf, sizeof(buf),
                 "GET %s HTTP/%s\r\n"
			     "Host: %s:%d\r\n"
				 "Connection: Close\r\n"
				 "User-Agent: " OS_STRING ", UPnP/1.0, MiniUPnPc/" MINIUPNPC_VERSION_STRING "\r\n"

				 "\r\n",
			   path, httpversion, host, port);
	sent = 0;
	 
	while(sent < len)
	{
		n = send(s, buf+sent, len-sent, 0);
		if(n < 0)
		{
			perror("send");
			closesocket(s);
			return NULL;
		}
		else
		{
			sent += n;
		}
	}
	content = getHTTPResponse(s, size);
	closesocket(s);
	return content;
}