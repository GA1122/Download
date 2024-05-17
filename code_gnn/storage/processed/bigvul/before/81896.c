int wc_ecc_import_private_key_ex(const byte* priv, word32 privSz,
                                 const byte* pub, word32 pubSz, ecc_key* key,
                                 int curve_id)
{
    int ret;
    word32 idx = 0;

    if (key == NULL || priv == NULL)
        return BAD_FUNC_ARG;

     
    if (pub != NULL) {
        ret = wc_ecc_import_x963_ex(pub, pubSz, key, curve_id);
        if (ret < 0)
            ret = wc_EccPublicKeyDecode(pub, &idx, key, pubSz);
        key->type = ECC_PRIVATEKEY;
    }
    else {
         
        wc_ecc_reset(key);

         
        ret = wc_ecc_set_curve(key, privSz, curve_id);
        key->type = ECC_PRIVATEKEY_ONLY;
    }

    if (ret != 0)
        return ret;

#ifdef WOLFSSL_ATECC508A
     
    return BAD_COND_E;

#else

    ret = mp_read_unsigned_bin(&key->k, priv, privSz);

#endif  

#ifdef WOLFSSL_VALIDATE_ECC_IMPORT
    if ((pub != NULL) && (ret == MP_OKAY))
         
        ret = ecc_check_privkey_gen_helper(key);
#endif

    return ret;
}
