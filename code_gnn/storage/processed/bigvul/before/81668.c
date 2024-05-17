forward_body_data(struct mg_connection *conn, FILE *fp, SOCKET sock, SSL *ssl)
{
	const char *expect, *body;
	char buf[MG_BUF_LEN];
	int to_read, nread, success = 0;
	int64_t buffered_len;
	double timeout = -1.0;

	if (!conn) {
		return 0;
	}
	if (conn->dom_ctx->config[REQUEST_TIMEOUT]) {
		timeout = atoi(conn->dom_ctx->config[REQUEST_TIMEOUT]) / 1000.0;
	}

	expect = mg_get_header(conn, "Expect");
	DEBUG_ASSERT(fp != NULL);
	if (!fp) {
		mg_send_http_error(conn, 500, "%s", "Error: NULL File");
		return 0;
	}

	if ((conn->content_len == -1) && (!conn->is_chunked)) {
		 
		mg_send_http_error(conn,
		                   411,
		                   "%s",
		                   "Error: Client did not specify content length");
	} else if ((expect != NULL)
	           && (mg_strcasecmp(expect, "100-continue") != 0)) {
		 
		mg_send_http_error(conn,
		                   417,
		                   "Error: Can not fulfill expectation %s",
		                   expect);
	} else {
		if (expect != NULL) {
			(void)mg_printf(conn, "%s", "HTTP/1.1 100 Continue\r\n\r\n");
			conn->status_code = 100;
		} else {
			conn->status_code = 200;
		}

		buffered_len = (int64_t)(conn->data_len) - (int64_t)conn->request_len
		               - conn->consumed_content;

		DEBUG_ASSERT(buffered_len >= 0);
		DEBUG_ASSERT(conn->consumed_content == 0);

		if ((buffered_len < 0) || (conn->consumed_content != 0)) {
			mg_send_http_error(conn, 500, "%s", "Error: Size mismatch");
			return 0;
		}

		if (buffered_len > 0) {
			if ((int64_t)buffered_len > conn->content_len) {
				buffered_len = (int)conn->content_len;
			}
			body = conn->buf + conn->request_len + conn->consumed_content;
			push_all(
			    conn->phys_ctx, fp, sock, ssl, body, (int64_t)buffered_len);
			conn->consumed_content += buffered_len;
		}

		nread = 0;
		while (conn->consumed_content < conn->content_len) {
			to_read = sizeof(buf);
			if ((int64_t)to_read > conn->content_len - conn->consumed_content) {
				to_read = (int)(conn->content_len - conn->consumed_content);
			}
			nread = pull_inner(NULL, conn, buf, to_read, timeout);
			if (nread == -2) {
				 
				break;
			}
			if (nread > 0) {
				if (push_all(conn->phys_ctx, fp, sock, ssl, buf, nread)
				    != nread) {
					break;
				}
			}
			conn->consumed_content += nread;
		}

		if (conn->consumed_content == conn->content_len) {
			success = (nread >= 0);
		}

		 
		if (!success) {
			 
			 
			mg_send_http_error(conn, 500, "%s", "");
		}
	}

	return success;
}
