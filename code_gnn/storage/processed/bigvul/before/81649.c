discard_unread_request_data(struct mg_connection *conn)
{
	char buf[MG_BUF_LEN];
	size_t to_read;
	int nread;

	if (conn == NULL) {
		return;
	}

	to_read = sizeof(buf);

	if (conn->is_chunked) {
		 
		while (conn->is_chunked != 3) {
			nread = mg_read(conn, buf, to_read);
			if (nread <= 0) {
				break;
			}
		}

	} else {
		 
		while (conn->consumed_content < conn->content_len) {
			if (to_read
			    > (size_t)(conn->content_len - conn->consumed_content)) {
				to_read = (size_t)(conn->content_len - conn->consumed_content);
			}

			nread = mg_read(conn, buf, to_read);
			if (nread <= 0) {
				break;
			}
		}
	}
}