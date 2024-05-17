void xdr_read_pages(struct xdr_stream *xdr, unsigned int len)
{
	struct xdr_buf *buf = xdr->buf;
	struct kvec *iov;
	ssize_t shift;
	unsigned int end;
	int padding;

	 
	iov  = buf->head;
	shift = iov->iov_len + (char *)iov->iov_base - (char *)xdr->p;
	if (shift > 0)
		xdr_shrink_bufhead(buf, shift);

	 
	if (buf->page_len > len)
		xdr_shrink_pagelen(buf, buf->page_len - len);
	padding = (XDR_QUADLEN(len) << 2) - len;
	xdr->iov = iov = buf->tail;
	 
	end = iov->iov_len;
	shift = buf->buflen - buf->len;
	if (shift < end)
		end -= shift;
	else if (shift > 0)
		end = 0;
	 
	xdr->p = (__be32 *)((char *)iov->iov_base + padding);
	xdr->end = (__be32 *)((char *)iov->iov_base + end);
}
