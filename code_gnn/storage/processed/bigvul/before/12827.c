int ASN1_INTEGER_set(ASN1_INTEGER *a, long v)
{
    int j, k;
    unsigned int i;
    unsigned char buf[sizeof(long) + 1];
    long d;

    a->type = V_ASN1_INTEGER;
    if (a->length < (int)(sizeof(long) + 1)) {
        if (a->data != NULL)
            OPENSSL_free(a->data);
        if ((a->data =
             (unsigned char *)OPENSSL_malloc(sizeof(long) + 1)) != NULL)
            memset((char *)a->data, 0, sizeof(long) + 1);
    }
    if (a->data == NULL) {
        ASN1err(ASN1_F_ASN1_INTEGER_SET, ERR_R_MALLOC_FAILURE);
        return (0);
    }
    d = v;
    if (d < 0) {
        d = -d;
        a->type = V_ASN1_NEG_INTEGER;
    }

    for (i = 0; i < sizeof(long); i++) {
        if (d == 0)
            break;
        buf[i] = (int)d & 0xff;
        d >>= 8;
    }
    j = 0;
    for (k = i - 1; k >= 0; k--)
        a->data[j++] = buf[k];
    a->length = j;
    return (1);
}
