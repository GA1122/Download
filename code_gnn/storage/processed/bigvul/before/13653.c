char *BN_bn2dec(const BIGNUM *a)
{
    int i = 0, num, ok = 0;
    char *buf = NULL;
    char *p;
    BIGNUM *t = NULL;
    BN_ULONG *bn_data = NULL, *lp;

     
    i = BN_num_bits(a) * 3;
    num = (i / 10 + i / 1000 + 1) + 1;
    bn_data =
        (BN_ULONG *)OPENSSL_malloc((num / BN_DEC_NUM + 1) * sizeof(BN_ULONG));
    buf = (char *)OPENSSL_malloc(num + 3);
    if ((buf == NULL) || (bn_data == NULL)) {
        BNerr(BN_F_BN_BN2DEC, ERR_R_MALLOC_FAILURE);
        goto err;
    }
    if ((t = BN_dup(a)) == NULL)
        goto err;

#define BUF_REMAIN (num+3 - (size_t)(p - buf))
    p = buf;
    lp = bn_data;
    if (BN_is_zero(t)) {
        *(p++) = '0';
        *(p++) = '\0';
    } else {
        if (BN_is_negative(t))
            *p++ = '-';

        i = 0;
        while (!BN_is_zero(t)) {
            *lp = BN_div_word(t, BN_DEC_CONV);
            lp++;
        }
        lp--;
         
        BIO_snprintf(p, BUF_REMAIN, BN_DEC_FMT1, *lp);
        while (*p)
            p++;
        while (lp != bn_data) {
            lp--;
            BIO_snprintf(p, BUF_REMAIN, BN_DEC_FMT2, *lp);
            while (*p)
                p++;
        }
    }
    ok = 1;
 err:
    if (bn_data != NULL)
        OPENSSL_free(bn_data);
    if (t != NULL)
        BN_free(t);
    if (!ok && buf) {
        OPENSSL_free(buf);
        buf = NULL;
    }

    return (buf);
}
