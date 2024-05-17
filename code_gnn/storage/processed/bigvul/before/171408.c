void remove_socket(asocket* s) {
 if (s->prev && s->next) {
        s->prev->next = s->next;
        s->next->prev = s->prev;
        s->next = 0;
        s->prev = 0;
        s->id = 0;
 }
}
