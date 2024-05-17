int tls1_process_ticket(SSL *s, unsigned char *session_id, int len,
                        const unsigned char *limit, SSL_SESSION **ret)
{
     
    const unsigned char *p = session_id + len;
    unsigned short i;

    *ret = NULL;
    s->tlsext_ticket_expected = 0;

     
    if (SSL_get_options(s) & SSL_OP_NO_TICKET)
        return 0;
    if ((s->version <= SSL3_VERSION) || !limit)
        return 0;
    if (p >= limit)
        return -1;
     
    if (SSL_IS_DTLS(s)) {
        i = *(p++);
        p += i;
        if (p >= limit)
            return -1;
    }
     
    n2s(p, i);
    p += i;
    if (p >= limit)
        return -1;
     
    i = *(p++);
    p += i;
    if (p > limit)
        return -1;
     
    if ((p + 2) >= limit)
        return 0;
    n2s(p, i);
    while ((p + 4) <= limit) {
        unsigned short type, size;
        n2s(p, type);
        n2s(p, size);
        if (p + size > limit)
            return 0;
        if (type == TLSEXT_TYPE_session_ticket) {
            int r;
            if (size == 0) {
                 
                s->tlsext_ticket_expected = 1;
                return 1;
            }
            if (s->tls_session_secret_cb) {
                 
                return 2;
            }
            r = tls_decrypt_ticket(s, p, size, session_id, len, ret);
            switch (r) {
            case 2:             
                s->tlsext_ticket_expected = 1;
                return 2;
            case 3:             
                return r;
            case 4:             
                s->tlsext_ticket_expected = 1;
                return 3;
            default:            
                return -1;
            }
        }
        p += size;
    }
    return 0;
}
