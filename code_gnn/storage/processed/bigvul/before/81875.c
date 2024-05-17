static int wc_ecc_export_raw(ecc_key* key, byte* qx, word32* qxLen,
                             byte* qy, word32* qyLen, byte* d, word32* dLen)
{
    int  err;
    byte exportPriv = 0;
    word32 numLen;

    if (key == NULL || qx == NULL || qxLen == NULL || qy == NULL ||
        qyLen == NULL) {
        return BAD_FUNC_ARG;
    }

    if (key->type == ECC_PRIVATEKEY_ONLY) {
        return ECC_PRIVATEONLY_E;
    }

    if (wc_ecc_is_valid_idx(key->idx) == 0) {
        return ECC_BAD_ARG_E;
    }
    numLen = key->dp->size;

    if (d != NULL) {
        if (dLen == NULL || key->type != ECC_PRIVATEKEY)
            return BAD_FUNC_ARG;
        exportPriv = 1;
    }

     
    if ((*qxLen < numLen) || (*qyLen < numLen)) {
        *qxLen = numLen;
        *qyLen = numLen;
        return BUFFER_E;
    }

    *qxLen = numLen;
    *qyLen = numLen;

    XMEMSET(qx, 0, *qxLen);
    XMEMSET(qy, 0, *qyLen);

     
    if (exportPriv == 1) {

         
        if (*dLen < numLen) {
            *dLen = numLen;
            return BUFFER_E;
        }

        *dLen = numLen;
        XMEMSET(d, 0, *dLen);

    #ifdef WOLFSSL_ATECC508A
        
       return BAD_COND_E;

    #else

         
        err = mp_to_unsigned_bin(&key->k, d +
                            (numLen - mp_unsigned_bin_size(&key->k)));
        if (err != MP_OKAY)
            return err;
    #endif  
    }

     
    err = mp_to_unsigned_bin(key->pubkey.x, qx +
                            (numLen - mp_unsigned_bin_size(key->pubkey.x)));
    if (err != MP_OKAY)
        return err;

     
    err = mp_to_unsigned_bin(key->pubkey.y, qy +
                            (numLen - mp_unsigned_bin_size(key->pubkey.y)));
    if (err != MP_OKAY)
        return err;

    return 0;
}