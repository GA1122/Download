int ssl2_new(SSL *s)
{
    SSL2_STATE *s2;

    if ((s2 = OPENSSL_malloc(sizeof *s2)) == NULL)
        goto err;
    memset(s2, 0, sizeof *s2);

# if SSL2_MAX_RECORD_LENGTH_3_BYTE_HEADER + 3 > SSL2_MAX_RECORD_LENGTH_2_BYTE_HEADER + 2
#  error "assertion failed"
# endif

    if ((s2->rbuf =
         OPENSSL_malloc(SSL2_MAX_RECORD_LENGTH_2_BYTE_HEADER + 2)) == NULL)
        goto err;
     
    if ((s2->wbuf =
         OPENSSL_malloc(SSL2_MAX_RECORD_LENGTH_2_BYTE_HEADER + 3)) == NULL)
        goto err;
    s->s2 = s2;

    ssl2_clear(s);
    return (1);
 err:
    if (s2 != NULL) {
        if (s2->wbuf != NULL)
            OPENSSL_free(s2->wbuf);
        if (s2->rbuf != NULL)
            OPENSSL_free(s2->rbuf);
        OPENSSL_free(s2);
    }
    return (0);
}
