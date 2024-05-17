static void smart_socket_close(asocket* s) {
    D("SS(%d): closed", s->id);
 if (s->pkt_first) {
        put_apacket(s->pkt_first);
 }
 if (s->peer) {
        s->peer->peer = 0;
        s->peer->close(s->peer);
        s->peer = 0;
 }
    free(s);
}
