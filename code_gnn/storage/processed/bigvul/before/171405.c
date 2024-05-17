static void remote_socket_close(asocket* s) {
 if (s->peer) {
        s->peer->peer = 0;
        D("RS(%d) peer->close()ing peer->id=%d peer->fd=%d", s->id, s->peer->id, s->peer->fd);
        s->peer->close(s->peer);
 }
    D("entered remote_socket_close RS(%d) CLOSE fd=%d peer->fd=%d", s->id, s->fd,
      s->peer ? s->peer->fd : -1);
    D("RS(%d): closed", s->id);
    free(s);
}
