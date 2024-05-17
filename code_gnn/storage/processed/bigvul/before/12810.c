int ssl_parse_clienthello_tlsext(SSL *s, unsigned char **p,
                                 unsigned char *limit)
{
    int al = -1;
    unsigned char *ptmp = *p;
     
    if (ssl_scan_clienthello_tlsext(s, p, limit, &al) <= 0) {
        ssl3_send_alert(s, SSL3_AL_FATAL, al);
        return 0;
    }

    if (ssl_check_clienthello_tlsext_early(s) <= 0) {
        SSLerr(SSL_F_SSL_PARSE_CLIENTHELLO_TLSEXT, SSL_R_CLIENTHELLO_TLSEXT);
        return 0;
    }

    custom_ext_init(&s->cert->srv_ext);
    if (ssl_scan_clienthello_custom_tlsext(s, ptmp, limit, &al) <= 0) {
        ssl3_send_alert(s, SSL3_AL_FATAL, al);
        return 0;
    }

    return 1;
}
