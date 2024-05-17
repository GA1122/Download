void connect_to_smartsocket(asocket* s) {
    D("Connecting to smart socket");
    asocket* ss = create_smart_socket();
    s->peer = ss;
    ss->peer = s;
    s->ready(s);
}
