static inline int http_parse_chunk_size(struct http_msg *msg)
{
	const struct buffer *buf = msg->chn->buf;
	const char *ptr = b_ptr(buf, msg->next);
	const char *ptr_old = ptr;
	const char *end = buf->data + buf->size;
	const char *stop = bi_end(buf);
	unsigned int chunk = 0;

	 
	while (1) {
		int c;
		if (ptr == stop)
			return 0;
		c = hex2i(*ptr);
		if (c < 0)  
			break;
		if (unlikely(++ptr >= end))
			ptr = buf->data;
		if (chunk & 0xF8000000)  
			goto error;
		chunk = (chunk << 4) + c;
	}

	 
	if (unlikely(ptr == ptr_old))
		goto error;

	while (http_is_spht[(unsigned char)*ptr]) {
		if (++ptr >= end)
			ptr = buf->data;
		if (unlikely(ptr == stop))
			return 0;
	}

	 
	while (1) {
		if (likely(HTTP_IS_CRLF(*ptr))) {
			 
			if (likely(*ptr == '\r')) {
				if (++ptr >= end)
					ptr = buf->data;
				if (ptr == stop)
					return 0;
			}

			if (*ptr != '\n')
				goto error;
			if (++ptr >= end)
				ptr = buf->data;
			 
			break;
		}
		else if (*ptr == ';') {
			 
			if (++ptr >= end)
				ptr = buf->data;
			if (ptr == stop)
				return 0;

			while (!HTTP_IS_CRLF(*ptr)) {
				if (++ptr >= end)
					ptr = buf->data;
				if (ptr == stop)
					return 0;
			}
			 
			continue;
		}
		else
			goto error;
	}

	 
	msg->sol = ptr - ptr_old;
	if (unlikely(ptr < ptr_old))
		msg->sol += buf->size;
	msg->next = buffer_count(buf, buf->p, ptr);
	msg->chunk_len = chunk;
	msg->body_len += chunk;
	msg->msg_state = chunk ? HTTP_MSG_DATA : HTTP_MSG_TRAILERS;
	return 1;
 error:
	msg->err_pos = buffer_count(buf, buf->p, ptr);
	return -1;
}
