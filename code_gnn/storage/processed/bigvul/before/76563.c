size_t nw_buf_write(nw_buf *buf, const void *data, size_t len)
{
    size_t available = buf->size - buf->wpos;
    size_t wlen = len > available ? available : len;
    memcpy(buf->data + buf->wpos, data, wlen);
    buf->wpos += wlen;
    return wlen;
}
