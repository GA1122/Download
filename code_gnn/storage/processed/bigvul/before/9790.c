static int http_forward_trailers(struct http_msg *msg)
{
	const struct buffer *buf = msg->chn->buf;

	 
	while (1) {
		const char *p1 = NULL, *p2 = NULL;
		const char *ptr = b_ptr(buf, msg->next);
		const char *stop = bi_end(buf);
		int bytes;

		 
		while (1) {
			if (ptr == stop)
				return 0;

			if (*ptr == '\n') {
				if (!p1)
					p1 = ptr;
				p2 = ptr;
				break;
			}

			if (*ptr == '\r') {
				if (p1) {
					msg->err_pos = buffer_count(buf, buf->p, ptr);
					return -1;
				}
				p1 = ptr;
			}

			ptr++;
			if (ptr >= buf->data + buf->size)
				ptr = buf->data;
		}

		 
		p2++;
		if (p2 >= buf->data + buf->size)
			p2 = buf->data;

		bytes = p2 - b_ptr(buf, msg->next);
		if (bytes < 0)
			bytes += buf->size;

		if (p1 == b_ptr(buf, msg->next)) {
			 
			msg->next = buffer_count(buf, buf->p, p2);
			msg->msg_state = HTTP_MSG_DONE;
			return 1;
		}
		 
		msg->next = buffer_count(buf, buf->p, p2);
	}
}
