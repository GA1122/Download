void ssl2_clear(SSL *s)
{
    SSL2_STATE *s2;
    unsigned char *rbuf, *wbuf;

    s2 = s->s2;

    rbuf = s2->rbuf;
    wbuf = s2->wbuf;

    memset(s2, 0, sizeof *s2);

    s2->rbuf = rbuf;
    s2->wbuf = wbuf;
    s2->clear_text = 1;
    s->packet = s2->rbuf;
    s->version = SSL2_VERSION;
    s->packet_length = 0;
}
