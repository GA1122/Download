 static JSON_INLINE void list_init(list_t *list)
 {
    list->next = list;
    list->prev = list;
}