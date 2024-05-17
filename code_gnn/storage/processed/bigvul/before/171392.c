asocket* create_remote_socket(unsigned id, atransport* t) {
 if (id == 0) {
        fatal("invalid remote socket id (0)");
 }
    asocket* s = reinterpret_cast<asocket*>(calloc(1, sizeof(asocket)));

 if (s == NULL) {
        fatal("cannot allocate socket");
 }
    s->id = id;
    s->enqueue = remote_socket_enqueue;
    s->ready = remote_socket_ready;
    s->shutdown = remote_socket_shutdown;
    s->close = remote_socket_close;
    s->transport = t;

    D("RS(%d): created", s->id);
 return s;
}
