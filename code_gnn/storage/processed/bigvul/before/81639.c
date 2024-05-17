connect_socket(struct mg_context *ctx  ,
               const char *host,
               int port,
               int use_ssl,
               char *ebuf,
               size_t ebuf_len,
               SOCKET *sock  ,
               union usa *sa  
               )
{
	int ip_ver = 0;
	int conn_ret = -1;
	*sock = INVALID_SOCKET;
	memset(sa, 0, sizeof(*sa));

	if (ebuf_len > 0) {
		*ebuf = 0;
	}

	if (host == NULL) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "NULL host");
		return 0;
	}

	if ((port <= 0) || !is_valid_port((unsigned)port)) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "invalid port");
		return 0;
	}

#if !defined(NO_SSL)
#if !defined(NO_SSL_DL)
#if defined(OPENSSL_API_1_1)
	if (use_ssl && (TLS_client_method == NULL)) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "SSL is not initialized");
		return 0;
	}
#else
	if (use_ssl && (SSLv23_client_method == NULL)) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "SSL is not initialized");
		return 0;
	}

#endif  
#else
	(void)use_ssl;
#endif  
#else
	(void)use_ssl;
#endif  

	if (mg_inet_pton(AF_INET, host, &sa->sin, sizeof(sa->sin))) {
		sa->sin.sin_family = AF_INET;
		sa->sin.sin_port = htons((uint16_t)port);
		ip_ver = 4;
#if defined(USE_IPV6)
	} else if (mg_inet_pton(AF_INET6, host, &sa->sin6, sizeof(sa->sin6))) {
		sa->sin6.sin6_family = AF_INET6;
		sa->sin6.sin6_port = htons((uint16_t)port);
		ip_ver = 6;
	} else if (host[0] == '[') {
		 
		size_t l = strlen(host + 1);
		char *h = (l > 1) ? mg_strdup_ctx(host + 1, ctx) : NULL;
		if (h) {
			h[l - 1] = 0;
			if (mg_inet_pton(AF_INET6, h, &sa->sin6, sizeof(sa->sin6))) {
				sa->sin6.sin6_family = AF_INET6;
				sa->sin6.sin6_port = htons((uint16_t)port);
				ip_ver = 6;
			}
			mg_free(h);
		}
#endif
	}

	if (ip_ver == 0) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "host not found");
		return 0;
	}

	if (ip_ver == 4) {
		*sock = socket(PF_INET, SOCK_STREAM, 0);
	}
#if defined(USE_IPV6)
	else if (ip_ver == 6) {
		*sock = socket(PF_INET6, SOCK_STREAM, 0);
	}
#endif

	if (*sock == INVALID_SOCKET) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "socket(): %s",
		            strerror(ERRNO));
		return 0;
	}

	if (0 != set_non_blocking_mode(*sock)) {
		mg_snprintf(NULL,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "Cannot set socket to non-blocking: %s",
		            strerror(ERRNO));
		closesocket(*sock);
		*sock = INVALID_SOCKET;
		return 0;
	}

	set_close_on_exec(*sock, fc(ctx));

	if (ip_ver == 4) {
		 
		conn_ret = connect(*sock,
		                   (struct sockaddr *)((void *)&sa->sin),
		                   sizeof(sa->sin));
	}
#if defined(USE_IPV6)
	else if (ip_ver == 6) {
		 
		conn_ret = connect(*sock,
		                   (struct sockaddr *)((void *)&sa->sin6),
		                   sizeof(sa->sin6));
	}
#endif

#if defined(_WIN32)
	if (conn_ret != 0) {
		DWORD err = WSAGetLastError();  
		conn_ret = (int)err;
#if !defined(EINPROGRESS)
#define EINPROGRESS (WSAEWOULDBLOCK)  
#endif                                
	}
#endif

	if ((conn_ret != 0) && (conn_ret != EINPROGRESS)) {
		 
		int sockerr = -1;
		void *psockerr = &sockerr;

#if defined(_WIN32)
		int len = (int)sizeof(sockerr);
#else
		socklen_t len = (socklen_t)sizeof(sockerr);
#endif

		 
		struct pollfd pfd[1];
		int pollres;
		int ms_wait = 10000;  

		 
		pfd[0].fd = *sock;
		pfd[0].events = POLLOUT;
		pollres = mg_poll(pfd, 1, (int)(ms_wait), &(ctx->stop_flag));

		if (pollres != 1) {
			 
			mg_snprintf(NULL,
			            NULL,  
			            ebuf,
			            ebuf_len,
			            "connect(%s:%d): timeout",
			            host,
			            port);
			closesocket(*sock);
			*sock = INVALID_SOCKET;
			return 0;
		}

#if defined(_WIN32)
		getsockopt(*sock, SOL_SOCKET, SO_ERROR, (char *)psockerr, &len);
#else
		getsockopt(*sock, SOL_SOCKET, SO_ERROR, psockerr, &len);
#endif

		if (sockerr != 0) {
			 
			mg_snprintf(NULL,
			            NULL,  
			            ebuf,
			            ebuf_len,
			            "connect(%s:%d): error %s",
			            host,
			            port,
			            strerror(sockerr));
			closesocket(*sock);
			*sock = INVALID_SOCKET;
			return 0;
		}
	}

	return 1;
}
