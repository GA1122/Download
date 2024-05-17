static void local_socket_close_notify(asocket* s) {
    s->ready = local_socket_ready;
    s->shutdown = NULL;
    s->close = local_socket_close;
 SendFail(s->fd, "closed");
    s->close(s);
}
