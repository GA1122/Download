nw_buf *nw_buf_alloc(nw_buf_pool *pool)
{
    if (pool->free) {
        nw_buf *buf = pool->free_arr[--pool->free];
        buf->size = pool->size;
        buf->rpos = 0;
        buf->wpos = 0;
        buf->next = NULL;
        return buf;
    }

    nw_buf *buf = malloc(sizeof(nw_buf) + pool->size);
    if (buf == NULL)
        return NULL;
    buf->size = pool->size;
    buf->rpos = 0;
    buf->wpos = 0;
    buf->next = NULL;

    return buf;
}
