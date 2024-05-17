nw_buf_list *nw_buf_list_create(nw_buf_pool *pool, uint32_t limit)
{
    nw_buf_list *list = malloc(sizeof(nw_buf_list));
    if (list == NULL)
        return NULL;
    list->pool = pool;
    list->count = 0;
    list->limit = limit;
    list->head = NULL;
    list->tail = NULL;

    return list;
}
