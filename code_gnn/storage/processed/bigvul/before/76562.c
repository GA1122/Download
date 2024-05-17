 size_t nw_buf_size(nw_buf *buf)
 {
    return buf->wpos - buf->rpos;
}
