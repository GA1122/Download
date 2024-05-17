asocket* create_local_socket(int fd) {
    asocket* s = reinterpret_cast<asocket*>(calloc(1, sizeof(asocket)));
 if (s == NULL) {
        fatal("cannot allocate socket");
 }
    s->fd = fd;
    s->enqueue = local_socket_enqueue;
    s->ready = local_socket_ready;
    s->shutdown = NULL;
    s->close = local_socket_close;
    install_local_socket(s);

    fdevent_install(&s->fde, fd, local_socket_event_func, s);
    D("LS(%d): created (fd=%d)", s->id, s->fd);
 return s;
}
