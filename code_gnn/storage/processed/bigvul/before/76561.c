void nw_buf_shift(nw_buf *buf)
{
    if (buf->rpos == buf->wpos) {
        buf->rpos = buf->wpos = 0;
    } else if (buf->rpos != 0) {
        memmove(buf->data, buf->data + buf->rpos, buf->wpos - buf->rpos);
        buf->wpos -= buf->rpos;
        buf->rpos = 0;
    }
}
