get_request_handler(struct mg_connection *conn,
                    int handler_type,
                    mg_request_handler *handler,
                    struct mg_websocket_subprotocols **subprotocols,
                    mg_websocket_connect_handler *connect_handler,
                    mg_websocket_ready_handler *ready_handler,
                    mg_websocket_data_handler *data_handler,
                    mg_websocket_close_handler *close_handler,
                    mg_authorization_handler *auth_handler,
                    void **cbdata,
                    struct mg_handler_info **handler_info)
{
	const struct mg_request_info *request_info = mg_get_request_info(conn);
	if (request_info) {
		const char *uri = request_info->local_uri;
		size_t urilen = strlen(uri);
		struct mg_handler_info *tmp_rh;

		if (!conn || !conn->phys_ctx || !conn->dom_ctx) {
			return 0;
		}

		mg_lock_context(conn->phys_ctx);

		 
		for (tmp_rh = conn->dom_ctx->handlers; tmp_rh != NULL;
		     tmp_rh = tmp_rh->next) {
			if (tmp_rh->handler_type == handler_type) {
				if ((urilen == tmp_rh->uri_len) && !strcmp(tmp_rh->uri, uri)) {
					if (handler_type == WEBSOCKET_HANDLER) {
						*subprotocols = tmp_rh->subprotocols;
						*connect_handler = tmp_rh->connect_handler;
						*ready_handler = tmp_rh->ready_handler;
						*data_handler = tmp_rh->data_handler;
						*close_handler = tmp_rh->close_handler;
					} else if (handler_type == REQUEST_HANDLER) {
						*handler = tmp_rh->handler;
						 
						handler_info_acquire(tmp_rh);
						*handler_info = tmp_rh;
					} else {  
						*auth_handler = tmp_rh->auth_handler;
					}
					*cbdata = tmp_rh->cbdata;
					mg_unlock_context(conn->phys_ctx);
					return 1;
				}
			}
		}

		 
		for (tmp_rh = conn->dom_ctx->handlers; tmp_rh != NULL;
		     tmp_rh = tmp_rh->next) {
			if (tmp_rh->handler_type == handler_type) {
				if ((tmp_rh->uri_len < urilen) && (uri[tmp_rh->uri_len] == '/')
				    && (memcmp(tmp_rh->uri, uri, tmp_rh->uri_len) == 0)) {
					if (handler_type == WEBSOCKET_HANDLER) {
						*subprotocols = tmp_rh->subprotocols;
						*connect_handler = tmp_rh->connect_handler;
						*ready_handler = tmp_rh->ready_handler;
						*data_handler = tmp_rh->data_handler;
						*close_handler = tmp_rh->close_handler;
					} else if (handler_type == REQUEST_HANDLER) {
						*handler = tmp_rh->handler;
						 
						handler_info_acquire(tmp_rh);
						*handler_info = tmp_rh;
					} else {  
						*auth_handler = tmp_rh->auth_handler;
					}
					*cbdata = tmp_rh->cbdata;
					mg_unlock_context(conn->phys_ctx);
					return 1;
				}
			}
		}

		 
		for (tmp_rh = conn->dom_ctx->handlers; tmp_rh != NULL;
		     tmp_rh = tmp_rh->next) {
			if (tmp_rh->handler_type == handler_type) {
				if (match_prefix(tmp_rh->uri, tmp_rh->uri_len, uri) > 0) {
					if (handler_type == WEBSOCKET_HANDLER) {
						*subprotocols = tmp_rh->subprotocols;
						*connect_handler = tmp_rh->connect_handler;
						*ready_handler = tmp_rh->ready_handler;
						*data_handler = tmp_rh->data_handler;
						*close_handler = tmp_rh->close_handler;
					} else if (handler_type == REQUEST_HANDLER) {
						*handler = tmp_rh->handler;
						 
						handler_info_acquire(tmp_rh);
						*handler_info = tmp_rh;
					} else {  
						*auth_handler = tmp_rh->auth_handler;
					}
					*cbdata = tmp_rh->cbdata;
					mg_unlock_context(conn->phys_ctx);
					return 1;
				}
			}
		}

		mg_unlock_context(conn->phys_ctx);
	}
	return 0;  
}