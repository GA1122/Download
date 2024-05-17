static int ssl3_set_req_cert_type(CERT *c, const unsigned char *p, size_t len)
{
    if (c->ctypes) {
        OPENSSL_free(c->ctypes);
        c->ctypes = NULL;
    }
    if (!p || !len)
        return 1;
    if (len > 0xff)
        return 0;
    c->ctypes = OPENSSL_malloc(len);
    if (!c->ctypes)
        return 0;
    memcpy(c->ctypes, p, len);
    c->ctype_num = len;
    return 1;
}
