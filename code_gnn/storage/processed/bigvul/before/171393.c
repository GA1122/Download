static asocket* create_smart_socket(void) {
    D("Creating smart socket");
    asocket* s = reinterpret_cast<asocket*>(calloc(1, sizeof(asocket)));
 if (s == NULL) fatal("cannot allocate socket");
    s->enqueue = smart_socket_enqueue;
    s->ready = smart_socket_ready;
    s->shutdown = NULL;
    s->close = smart_socket_close;

    D("SS(%d)", s->id);
 return s;
}
