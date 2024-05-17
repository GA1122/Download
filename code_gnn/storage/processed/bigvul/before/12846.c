static int der_cmp(const void *a, const void *b)
{
    const DER_ENC *d1 = a, *d2 = b;
    int cmplen, i;
    cmplen = (d1->length < d2->length) ? d1->length : d2->length;
    i = memcmp(d1->data, d2->data, cmplen);
    if (i)
        return i;
    return d1->length - d2->length;
}
