void nw_buf_list_shift(nw_buf_list *list)
{
    if (list->head) {
        nw_buf *tmp = list->head;
        list->head = tmp->next;
        if (list->head == NULL) {
            list->tail = NULL;
        }
        list->count--;
        nw_buf_free(list->pool, tmp);
    }
}
