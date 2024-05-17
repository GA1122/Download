have_handshake_fragment(SSL *s, int type, unsigned char *buf,
                        int len, int peek)
{

    if ((type == SSL3_RT_HANDSHAKE) && (s->d1->handshake_fragment_len > 0))
         
    {
        unsigned char *src = s->d1->handshake_fragment;
        unsigned char *dst = buf;
        unsigned int k, n;

         
        n = 0;
        while ((len > 0) && (s->d1->handshake_fragment_len > 0)) {
            *dst++ = *src++;
            len--;
            s->d1->handshake_fragment_len--;
            n++;
        }
         
        for (k = 0; k < s->d1->handshake_fragment_len; k++)
            s->d1->handshake_fragment[k] = *src++;
        return n;
    }

    return 0;
}
