accept_new_connection(const struct socket *listener, struct mg_context *ctx)
{
	struct socket so;
	char src_addr[IP_ADDR_STR_LEN];
	socklen_t len = sizeof(so.rsa);
	int on = 1;

	if ((so.sock = accept(listener->sock, &so.rsa.sa, &len))
	    == INVALID_SOCKET) {
	} else if (!check_acl(ctx, ntohl(*(uint32_t *)&so.rsa.sin.sin_addr))) {
		sockaddr_to_string(src_addr, sizeof(src_addr), &so.rsa);
		mg_cry_internal(fc(ctx),
		                "%s: %s is not allowed to connect",
		                __func__,
		                src_addr);
		closesocket(so.sock);
	} else {
		 
		DEBUG_TRACE("Accepted socket %d", (int)so.sock);
		set_close_on_exec(so.sock, fc(ctx));
		so.is_ssl = listener->is_ssl;
		so.ssl_redir = listener->ssl_redir;
		if (getsockname(so.sock, &so.lsa.sa, &len) != 0) {
			mg_cry_internal(fc(ctx),
			                "%s: getsockname() failed: %s",
			                __func__,
			                strerror(ERRNO));
		}

		 
		if (setsockopt(so.sock,
		               SOL_SOCKET,
		               SO_KEEPALIVE,
		               (SOCK_OPT_TYPE)&on,
		               sizeof(on)) != 0) {
			mg_cry_internal(
			    fc(ctx),
			    "%s: setsockopt(SOL_SOCKET SO_KEEPALIVE) failed: %s",
			    __func__,
			    strerror(ERRNO));
		}

		 
		if ((ctx->dd.config[CONFIG_TCP_NODELAY] != NULL)
		    && (!strcmp(ctx->dd.config[CONFIG_TCP_NODELAY], "1"))) {
			if (set_tcp_nodelay(so.sock, 1) != 0) {
				mg_cry_internal(
				    fc(ctx),
				    "%s: setsockopt(IPPROTO_TCP TCP_NODELAY) failed: %s",
				    __func__,
				    strerror(ERRNO));
			}
		}

		 

		 
		set_non_blocking_mode(so.sock);

		so.in_use = 0;
		produce_socket(ctx, &so);
	}
}
