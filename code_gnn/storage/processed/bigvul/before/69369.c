int ssl_cipher_list_to_bytes(SSL *s, STACK_OF(SSL_CIPHER) *sk, unsigned char *p)
{
    int i, j = 0;
    const SSL_CIPHER *c;
    unsigned char *q;
    int empty_reneg_info_scsv = !s->renegotiate;
     
    ssl_set_client_disabled(s);

    if (sk == NULL)
        return (0);
    q = p;

    for (i = 0; i < sk_SSL_CIPHER_num(sk); i++) {
        c = sk_SSL_CIPHER_value(sk, i);
         
        if (ssl_cipher_disabled(s, c, SSL_SECOP_CIPHER_SUPPORTED))
            continue;
        j = s->method->put_cipher_by_char(c, p);
        p += j;
    }
     
    if (p != q) {
        if (empty_reneg_info_scsv) {
            static SSL_CIPHER scsv = {
                0, NULL, SSL3_CK_SCSV, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            j = s->method->put_cipher_by_char(&scsv, p);
            p += j;
        }
        if (s->mode & SSL_MODE_SEND_FALLBACK_SCSV) {
            static SSL_CIPHER scsv = {
                0, NULL, SSL3_CK_FALLBACK_SCSV, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            j = s->method->put_cipher_by_char(&scsv, p);
            p += j;
        }
    }

    return (p - q);
}