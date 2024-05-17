int xdr_buf_read_netobj(struct xdr_buf *buf, struct xdr_netobj *obj, unsigned int offset)
{
	struct xdr_buf subbuf;

	if (xdr_decode_word(buf, offset, &obj->len))
		return -EFAULT;
	if (xdr_buf_subsegment(buf, &subbuf, offset + 4, obj->len))
		return -EFAULT;

	 
	obj->data = subbuf.head[0].iov_base;
	if (subbuf.head[0].iov_len == obj->len)
		return 0;
	 
	obj->data = subbuf.tail[0].iov_base;
	if (subbuf.tail[0].iov_len == obj->len)
		return 0;

	 
	if (obj->len > buf->buflen - buf->len)
		return -ENOMEM;
	if (buf->tail[0].iov_len != 0)
		obj->data = buf->tail[0].iov_base + buf->tail[0].iov_len;
	else
		obj->data = buf->head[0].iov_base + buf->head[0].iov_len;
	__read_bytes_from_xdr_buf(&subbuf, obj->data, obj->len);
	return 0;
}
