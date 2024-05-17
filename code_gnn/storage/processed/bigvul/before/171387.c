void connect_to_remote(asocket* s, const char* destination) {
    D("Connect_to_remote call RS(%d) fd=%d", s->id, s->fd);
    apacket* p = get_apacket();
 size_t len = strlen(destination) + 1;

 if (len > (s->get_max_payload() - 1)) {
        fatal("destination oversized");
 }

    D("LS(%d): connect('%s')", s->id, destination);
    p->msg.command = A_OPEN;
    p->msg.arg0 = s->id;
    p->msg.data_length = len;
    strcpy((char*)p->data, destination);
    send_packet(p, s->transport);
}
