size_t nw_buf_list_append(nw_buf_list *list, const void *data, size_t len)
{
    if (list->limit && list->count >= list->limit)
        return 0;
    nw_buf *buf = nw_buf_alloc(list->pool);
    if (buf == NULL)
        return 0;
    if (len > buf->size) {
        nw_buf_free(list->pool, buf);
        return 0;
    }
    nw_buf_write(buf, data, len);
    if (list->head == NULL)
        list->head = buf;
    if (list->tail != NULL)
        list->tail->next = buf;
    list->tail = buf;
    list->count++;

    return len;
}
