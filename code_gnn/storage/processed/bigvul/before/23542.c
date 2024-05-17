xdr_shrink_pagelen(struct xdr_buf *buf, size_t len)
{
	struct kvec *tail;
	size_t copy;
	unsigned int pglen = buf->page_len;
	unsigned int tailbuf_len;

	tail = buf->tail;
	BUG_ON (len > pglen);

	tailbuf_len = buf->buflen - buf->head->iov_len - buf->page_len;

	 
	if (tailbuf_len != 0) {
		unsigned int free_space = tailbuf_len - tail->iov_len;

		if (len < free_space)
			free_space = len;
		tail->iov_len += free_space;

		copy = len;
		if (tail->iov_len > len) {
			char *p = (char *)tail->iov_base + len;
			memmove(p, tail->iov_base, tail->iov_len - len);
		} else
			copy = tail->iov_len;
		 
		_copy_from_pages((char *)tail->iov_base,
				buf->pages, buf->page_base + pglen - len,
				copy);
	}
	buf->page_len -= len;
	buf->buflen -= len;
	 
	if (buf->len > buf->buflen)
		buf->len = buf->buflen;
}
