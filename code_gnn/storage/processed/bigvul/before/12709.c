int dtls1_process_heartbeat(SSL *s, unsigned char *p, unsigned int length)
{
    unsigned char *pl;
    unsigned short hbtype;
    unsigned int payload;
    unsigned int padding = 16;   

    if (s->msg_callback)
        s->msg_callback(0, s->version, DTLS1_RT_HEARTBEAT,
                        p, length, s, s->msg_callback_arg);

     
    if (HEARTBEAT_SIZE_STD(0) > length)
        return 0;                
    if (length > SSL3_RT_MAX_PLAIN_LENGTH)
        return 0;                

    hbtype = *p++;
    n2s(p, payload);
    if (HEARTBEAT_SIZE_STD(payload) > length)
        return 0;                
    pl = p;

    if (hbtype == TLS1_HB_REQUEST) {
        unsigned char *buffer, *bp;
        unsigned int write_length = HEARTBEAT_SIZE(payload, padding);
        int r;

        if (write_length > SSL3_RT_MAX_PLAIN_LENGTH)
            return 0;

         
        buffer = OPENSSL_malloc(write_length);
        if (buffer == NULL)
            return -1;
        bp = buffer;

         
        *bp++ = TLS1_HB_RESPONSE;
        s2n(payload, bp);
        memcpy(bp, pl, payload);
        bp += payload;
         
        if (RAND_bytes(bp, padding) <= 0) {
            OPENSSL_free(buffer);
            return -1;
        }

        r = dtls1_write_bytes(s, DTLS1_RT_HEARTBEAT, buffer, write_length);

        if (r >= 0 && s->msg_callback)
            s->msg_callback(1, s->version, DTLS1_RT_HEARTBEAT,
                            buffer, write_length, s, s->msg_callback_arg);

        OPENSSL_free(buffer);

        if (r < 0)
            return r;
    } else if (hbtype == TLS1_HB_RESPONSE) {
        unsigned int seq;

         
        n2s(pl, seq);

        if (payload == 18 && seq == s->tlsext_hb_seq) {
            dtls1_stop_timer(s);
            s->tlsext_hb_seq++;
            s->tlsext_hb_pending = 0;
        }
    }

    return 0;
}
