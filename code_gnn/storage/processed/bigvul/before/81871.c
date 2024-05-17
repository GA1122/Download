int wc_ecc_export_point_der(const int curve_idx, ecc_point* point, byte* out,
                            word32* outLen)
{
    int    ret = MP_OKAY;
    word32 numlen;
#ifndef WOLFSSL_ATECC508A
#ifdef WOLFSSL_SMALL_STACK
    byte*  buf;
#else
    byte   buf[ECC_BUFSIZE];
#endif
#endif  

    if ((curve_idx < 0) || (wc_ecc_is_valid_idx(curve_idx) == 0))
        return ECC_BAD_ARG_E;

     
    if (point != NULL && out == NULL && outLen != NULL) {
        numlen = ecc_sets[curve_idx].size;
        *outLen = 1 + 2*numlen;
        return LENGTH_ONLY_E;
    }

    if (point == NULL || out == NULL || outLen == NULL)
        return ECC_BAD_ARG_E;

    numlen = ecc_sets[curve_idx].size;

    if (*outLen < (1 + 2*numlen)) {
        *outLen = 1 + 2*numlen;
        return BUFFER_E;
    }

#ifdef WOLFSSL_ATECC508A
    
   ret = BAD_COND_E;

#else

     
    out[0] = ECC_POINT_UNCOMP;

#ifdef WOLFSSL_SMALL_STACK
    buf = (byte*)XMALLOC(ECC_BUFSIZE, NULL, DYNAMIC_TYPE_ECC_BUFFER);
    if (buf == NULL)
        return MEMORY_E;
#endif

     
    XMEMSET(buf, 0, ECC_BUFSIZE);
    ret = mp_to_unsigned_bin(point->x, buf +
                                 (numlen - mp_unsigned_bin_size(point->x)));
    if (ret != MP_OKAY)
        goto done;
    XMEMCPY(out+1, buf, numlen);

     
    XMEMSET(buf, 0, ECC_BUFSIZE);
    ret = mp_to_unsigned_bin(point->y, buf +
                                 (numlen - mp_unsigned_bin_size(point->y)));
    if (ret != MP_OKAY)
        goto done;
    XMEMCPY(out+1+numlen, buf, numlen);

    *outLen = 1 + 2*numlen;

done:
#ifdef WOLFSSL_SMALL_STACK
    XFREE(buf, NULL, DYNAMIC_TYPE_ECC_BUFFER);
#endif
#endif  

    return ret;
}