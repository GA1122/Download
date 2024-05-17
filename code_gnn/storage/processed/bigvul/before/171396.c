static void insert_local_socket(asocket* s, asocket* list) {
    s->next = list;
    s->prev = s->next->prev;
    s->prev->next = s;
    s->next->prev = s;
}
