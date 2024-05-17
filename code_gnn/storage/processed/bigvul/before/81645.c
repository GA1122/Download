deprecated_websocket_connect_wrapper(const struct mg_connection *conn,
                                     void *cbdata)
{
	struct mg_callbacks *pcallbacks = (struct mg_callbacks *)cbdata;
	if (pcallbacks->websocket_connect) {
		return pcallbacks->websocket_connect(conn);
	}
	 
	return 0;
}