long ASN1_INTEGER_get(const ASN1_INTEGER *a)
{
    int neg = 0, i;
    long r = 0;

    if (a == NULL)
        return (0L);
    i = a->type;
    if (i == V_ASN1_NEG_INTEGER)
        neg = 1;
    else if (i != V_ASN1_INTEGER)
        return -1;

    if (a->length > (int)sizeof(long)) {
         
        return -1;
    }
    if (a->data == NULL)
        return 0;

    for (i = 0; i < a->length; i++) {
        r <<= 8;
        r |= (unsigned char)a->data[i];
    }
    if (neg)
        r = -r;
    return (r);
}
