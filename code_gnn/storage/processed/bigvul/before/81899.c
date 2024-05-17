static int wc_ecc_import_raw_private(ecc_key* key, const char* qx,
          const char* qy, const char* d, int curve_id, int encType)
{
    int err = MP_OKAY;

     
    if (key == NULL || qx == NULL || qy == NULL) {
        return BAD_FUNC_ARG;
    }

     
    wc_ecc_reset(key);

     
    err = wc_ecc_set_curve(key, 0, curve_id);
    if (err != 0) {
        return err;
    }

#ifdef WOLFSSL_ATECC508A
     
    err = BAD_COND_E;

#else

     
#ifdef ALT_ECC_SIZE
    key->pubkey.x = (mp_int*)&key->pubkey.xyz[0];
    key->pubkey.y = (mp_int*)&key->pubkey.xyz[1];
    key->pubkey.z = (mp_int*)&key->pubkey.xyz[2];
    alt_fp_init(key->pubkey.x);
    alt_fp_init(key->pubkey.y);
    alt_fp_init(key->pubkey.z);
    err = mp_init(&key->k);
#else
    err = mp_init_multi(&key->k, key->pubkey.x, key->pubkey.y, key->pubkey.z,
                                                                  NULL, NULL);
#endif
    if (err != MP_OKAY)
        return MEMORY_E;

     
    if (err == MP_OKAY) {
        if (encType == ECC_TYPE_HEX_STR)
            err = mp_read_radix(key->pubkey.x, qx, MP_RADIX_HEX);
        else
            err = mp_read_unsigned_bin(key->pubkey.x, (const byte*)qx,
                key->dp->size);
    }

     
    if (err == MP_OKAY) {
        if (encType == ECC_TYPE_HEX_STR)
            err = mp_read_radix(key->pubkey.y, qy, MP_RADIX_HEX);
        else
            err = mp_read_unsigned_bin(key->pubkey.y, (const byte*)qy,
                key->dp->size);

    }

    if (err == MP_OKAY)
        err = mp_set(key->pubkey.z, 1);

     
    if (err == MP_OKAY) {
        if (d != NULL) {
            key->type = ECC_PRIVATEKEY;

            if (encType == ECC_TYPE_HEX_STR)
                err = mp_read_radix(&key->k, d, MP_RADIX_HEX);
            else
                err = mp_read_unsigned_bin(&key->k, (const byte*)d,
                    key->dp->size);

        } else {
            key->type = ECC_PUBLICKEY;
        }
    }

#ifdef WOLFSSL_VALIDATE_ECC_IMPORT
    if (err == MP_OKAY)
        err = wc_ecc_check_key(key);
#endif

    if (err != MP_OKAY) {
        mp_clear(key->pubkey.x);
        mp_clear(key->pubkey.y);
        mp_clear(key->pubkey.z);
        mp_clear(&key->k);
    }
#endif  

    return err;
}