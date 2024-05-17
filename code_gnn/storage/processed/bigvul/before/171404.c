static void local_socket_ready_notify(asocket* s) {
    s->ready = local_socket_ready;
    s->shutdown = NULL;
    s->close = local_socket_close;
 SendOkay(s->fd);
    s->ready(s);
}
