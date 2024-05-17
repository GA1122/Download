deprecated_websocket_data_wrapper(struct mg_connection *conn,
                                  int bits,
                                  char *data,
                                  size_t len,
                                  void *cbdata)
{
	struct mg_callbacks *pcallbacks = (struct mg_callbacks *)cbdata;
	if (pcallbacks->websocket_data) {
		return pcallbacks->websocket_data(conn, bits, data, len);
	}
	 
	return 1;
}