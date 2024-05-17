size_t nw_buf_list_write(nw_buf_list *list, const void *data, size_t len)
{
    const void *pos = data;
    size_t left = len;

    if (list->tail && nw_buf_avail(list->tail)) {
        size_t ret = nw_buf_write(list->tail, pos, left);
        left -= ret;
        pos += ret;
    }

    while (left) {
        if (list->limit && list->count >= list->limit)
            return len - left;
        nw_buf *buf = nw_buf_alloc(list->pool);
        if (buf == NULL)
            return len - left;
        if (list->head == NULL)
            list->head = buf;
        if (list->tail != NULL)
            list->tail->next = buf;
        list->tail = buf;
        list->count++;
        size_t ret = nw_buf_write(list->tail, pos, left);
        left -= ret;
        pos += ret;
    }

    return len;
}
