get_response(struct mg_connection *conn, char *ebuf, size_t ebuf_len, int *err)
{
	const char *cl;
	if (!get_message(conn, ebuf, ebuf_len, err)) {
		return 0;
	}

	if (parse_http_response(conn->buf, conn->buf_size, &conn->response_info)
	    <= 0) {
		mg_snprintf(conn,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "Bad response");
		*err = 400;
		return 0;
	}

	 

	 
	if ((cl = get_header(conn->response_info.http_headers,
	                     conn->response_info.num_headers,
	                     "Content-Length")) != NULL) {
		 
		char *endptr = NULL;
		conn->content_len = strtoll(cl, &endptr, 10);
		if (endptr == cl) {
			mg_snprintf(conn,
			            NULL,  
			            ebuf,
			            ebuf_len,
			            "%s",
			            "Bad request");
			*err = 411;
			return 0;
		}
		 
		conn->response_info.content_length = conn->content_len;

		 
		conn->request_info.content_length = conn->content_len;

	} else if ((cl = get_header(conn->response_info.http_headers,
	                            conn->response_info.num_headers,
	                            "Transfer-Encoding")) != NULL
	           && !mg_strcasecmp(cl, "chunked")) {
		conn->is_chunked = 1;
		conn->content_len = -1;  
	} else {
		conn->content_len = -1;  
	}

	conn->connection_type = CONNECTION_TYPE_RESPONSE;  
	return 1;
}
