get_request(struct mg_connection *conn, char *ebuf, size_t ebuf_len, int *err)
{
	const char *cl;
	if (!get_message(conn, ebuf, ebuf_len, err)) {
		return 0;
	}

	if (parse_http_request(conn->buf, conn->buf_size, &conn->request_info)
	    <= 0) {
		mg_snprintf(conn,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "Bad request");
		*err = 400;
		return 0;
	}

	 

	 
	conn->host = alloc_get_host(conn);
	if (!conn->host) {
		mg_snprintf(conn,
		            NULL,  
		            ebuf,
		            ebuf_len,
		            "%s",
		            "Bad request: Host mismatch");
		*err = 400;
		return 0;
	}

	 
	if ((cl = get_header(conn->request_info.http_headers,
	                     conn->request_info.num_headers,
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
		 
		conn->request_info.content_length = conn->content_len;
	} else if ((cl = get_header(conn->request_info.http_headers,
	                            conn->request_info.num_headers,
	                            "Transfer-Encoding")) != NULL
	           && !mg_strcasecmp(cl, "chunked")) {
		conn->is_chunked = 1;
		conn->content_len = -1;  
	} else {
		const struct mg_http_method_info *meth =
		    get_http_method_info(conn->request_info.request_method);
		if (!meth) {
			 
			mg_snprintf(conn,
			            NULL,  
			            ebuf,
			            ebuf_len,
			            "%s",
			            "Bad request");
			*err = 411;
			return 0;
		}
		if (meth->request_has_body) {
			 
			conn->content_len = -1;  
		} else {
			 
			conn->content_len = 0;  
		}
	}

	conn->connection_type = CONNECTION_TYPE_REQUEST;  
	return 1;
}
