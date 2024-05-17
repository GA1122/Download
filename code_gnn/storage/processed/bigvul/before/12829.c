ASN1_INTEGER *c2i_ASN1_INTEGER(ASN1_INTEGER **a, const unsigned char **pp,
                               long len)
{
    ASN1_INTEGER *ret = NULL;
    const unsigned char *p, *pend;
    unsigned char *to, *s;
    int i;

    if ((a == NULL) || ((*a) == NULL)) {
        if ((ret = M_ASN1_INTEGER_new()) == NULL)
            return (NULL);
        ret->type = V_ASN1_INTEGER;
    } else
        ret = (*a);

    p = *pp;
    pend = p + len;

     
    s = (unsigned char *)OPENSSL_malloc((int)len + 1);
    if (s == NULL) {
        i = ERR_R_MALLOC_FAILURE;
        goto err;
    }
    to = s;
    if (!len) {
         
        ret->type = V_ASN1_INTEGER;
    } else if (*p & 0x80) {      
        ret->type = V_ASN1_NEG_INTEGER;
        if ((*p == 0xff) && (len != 1)) {
            p++;
            len--;
        }
        i = len;
        p += i - 1;
        to += i - 1;
        while ((!*p) && i) {
            *(to--) = 0;
            i--;
            p--;
        }
         
        if (!i) {
            *s = 1;
            s[len] = 0;
            len++;
        } else {
            *(to--) = (*(p--) ^ 0xff) + 1;
            i--;
            for (; i > 0; i--)
                *(to--) = *(p--) ^ 0xff;
        }
    } else {
        ret->type = V_ASN1_INTEGER;
        if ((*p == 0) && (len != 1)) {
            p++;
            len--;
        }
        memcpy(s, p, (int)len);
    }

    if (ret->data != NULL)
        OPENSSL_free(ret->data);
    ret->data = s;
    ret->length = (int)len;
    if (a != NULL)
        (*a) = ret;
    *pp = pend;
    return (ret);
 err:
    ASN1err(ASN1_F_C2I_ASN1_INTEGER, i);
    if ((ret != NULL) && ((a == NULL) || (*a != ret)))
        M_ASN1_INTEGER_free(ret);
    return (NULL);
}
