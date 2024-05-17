close_socket_gracefully(struct mg_connection *conn)
{
#if defined(_WIN32)
	char buf[MG_BUF_LEN];
	int n;
#endif
	struct linger linger;
	int error_code = 0;
	int linger_timeout = -2;
	socklen_t opt_len = sizeof(error_code);

	if (!conn) {
		return;
	}

	 
	set_blocking_mode(conn->client.sock);

	 
	shutdown(conn->client.sock, SHUTDOWN_WR);


#if defined(_WIN32)
	 
	do {
		n = pull_inner(NULL, conn, buf, sizeof(buf),   1.0);
	} while (n > 0);
#endif

	if (conn->dom_ctx->config[LINGER_TIMEOUT]) {
		linger_timeout = atoi(conn->dom_ctx->config[LINGER_TIMEOUT]);
	}

	 
	if (linger_timeout >= 0) {
		 
		linger.l_onoff = 1;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4244)
#endif
#if defined(__GNUC__) || defined(__MINGW32__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
		 

		linger.l_linger = (linger_timeout + 999) / 1000;

#if defined(__GNUC__) || defined(__MINGW32__)
#pragma GCC diagnostic pop
#endif
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

	} else {
		linger.l_onoff = 0;
		linger.l_linger = 0;
	}

	if (linger_timeout < -1) {
		 
	} else if (getsockopt(conn->client.sock,
	                      SOL_SOCKET,
	                      SO_ERROR,
#if defined(_WIN32)  
	                      (char *)&error_code,
#else
	                      &error_code,
#endif
	                      &opt_len) != 0) {
		 
		mg_cry_internal(conn,
		                "%s: getsockopt(SOL_SOCKET SO_ERROR) failed: %s",
		                __func__,
		                strerror(ERRNO));
	} else if (error_code == ECONNRESET) {
		 
	} else {

		 
		if (setsockopt(conn->client.sock,
		               SOL_SOCKET,
		               SO_LINGER,
		               (char *)&linger,
		               sizeof(linger)) != 0) {
			mg_cry_internal(
			    conn,
			    "%s: setsockopt(SOL_SOCKET SO_LINGER(%i,%i)) failed: %s",
			    __func__,
			    linger.l_onoff,
			    linger.l_linger,
			    strerror(ERRNO));
		}
	}

	 
	closesocket(conn->client.sock);
	conn->client.sock = INVALID_SOCKET;
}
