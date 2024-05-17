close_connection(struct mg_connection *conn)
{
#if defined(USE_SERVER_STATS)
	conn->conn_state = 6;  
#endif

#if defined(USE_LUA) && defined(USE_WEBSOCKET)
	if (conn->lua_websocket_state) {
		lua_websocket_close(conn, conn->lua_websocket_state);
		conn->lua_websocket_state = NULL;
	}
#endif

	mg_lock_connection(conn);

	 
	conn->must_close = 1;

	 
	if (conn->phys_ctx->callbacks.connection_close != NULL) {
		if (conn->phys_ctx->context_type == CONTEXT_SERVER) {
			conn->phys_ctx->callbacks.connection_close(conn);
		}
	}

	 
	mg_set_user_connection_data(conn, NULL);


#if defined(USE_SERVER_STATS)
	conn->conn_state = 7;  
#endif

#if !defined(NO_SSL)
	if (conn->ssl != NULL) {
		 
		SSL_shutdown(conn->ssl);
		SSL_free(conn->ssl);
 
#if !defined(OPENSSL_API_1_1)
		ERR_remove_state(0);
#endif
		conn->ssl = NULL;
	}
#endif
	if (conn->client.sock != INVALID_SOCKET) {
		close_socket_gracefully(conn);
		conn->client.sock = INVALID_SOCKET;
	}

	if (conn->host) {
		mg_free((void *)conn->host);
		conn->host = NULL;
	}

	mg_unlock_connection(conn);

#if defined(USE_SERVER_STATS)
	conn->conn_state = 8;  
#endif
}
