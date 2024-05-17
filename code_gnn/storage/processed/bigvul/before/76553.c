size_t nw_buf_avail(nw_buf *buf)
{
    return buf->size - buf->wpos;
}
