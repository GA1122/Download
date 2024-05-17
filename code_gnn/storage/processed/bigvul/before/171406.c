static int remote_socket_enqueue(asocket* s, apacket* p) {
    D("entered remote_socket_enqueue RS(%d) WRITE fd=%d peer.fd=%d", s->id, s->fd, s->peer->fd);
    p->msg.command = A_WRTE;
    p->msg.arg0 = s->peer->id;
    p->msg.arg1 = s->id;
    p->msg.data_length = p->len;
    send_packet(p, s->transport);
 return 1;
}
