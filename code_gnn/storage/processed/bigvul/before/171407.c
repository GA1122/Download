static void remote_socket_shutdown(asocket* s) {
    D("entered remote_socket_shutdown RS(%d) CLOSE fd=%d peer->fd=%d", s->id, s->fd,
      s->peer ? s->peer->fd : -1);
    apacket* p = get_apacket();
    p->msg.command = A_CLSE;
 if (s->peer) {
        p->msg.arg0 = s->peer->id;
 }
    p->msg.arg1 = s->id;
    send_packet(p, s->transport);
}
