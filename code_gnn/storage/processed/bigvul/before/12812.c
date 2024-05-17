static int tls1_alpn_handle_client_hello(SSL *s, const unsigned char *data,
                                         unsigned data_len, int *al)
{
    unsigned i;
    unsigned proto_len;

    if (data_len < 2)
        goto parse_error;

     
    i = ((unsigned)data[0]) << 8 | ((unsigned)data[1]);
    data_len -= 2;
    data += 2;
    if (data_len != i)
        goto parse_error;

    if (data_len < 2)
        goto parse_error;

    for (i = 0; i < data_len;) {
        proto_len = data[i];
        i++;

        if (proto_len == 0)
            goto parse_error;

        if (i + proto_len < i || i + proto_len > data_len)
            goto parse_error;

        i += proto_len;
    }

    if (s->cert->alpn_proposed != NULL)
        OPENSSL_free(s->cert->alpn_proposed);
    s->cert->alpn_proposed = OPENSSL_malloc(data_len);
    if (s->cert->alpn_proposed == NULL) {
        *al = SSL_AD_INTERNAL_ERROR;
        return -1;
    }
    memcpy(s->cert->alpn_proposed, data, data_len);
    s->cert->alpn_proposed_len = data_len;
    return 0;

 parse_error:
    *al = SSL_AD_DECODE_ERROR;
    return -1;
}
