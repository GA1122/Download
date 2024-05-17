int wc_ecc_import_x963_ex(const byte* in, word32 inLen, ecc_key* key,
                          int curve_id)
{
    int err = MP_OKAY;
    int compressed = 0;
    int keysize = 0;
    byte pointType;

    if (in == NULL || key == NULL)
        return BAD_FUNC_ARG;

     
    if ((inLen & 1) == 0) {
        return ECC_BAD_ARG_E;
    }

     
    wc_ecc_reset(key);

     
    #ifdef ALT_ECC_SIZE
        key->pubkey.x = (mp_int*)&key->pubkey.xyz[0];
        key->pubkey.y = (mp_int*)&key->pubkey.xyz[1];
        key->pubkey.z = (mp_int*)&key->pubkey.xyz[2];
        alt_fp_init(key->pubkey.x);
        alt_fp_init(key->pubkey.y);
        alt_fp_init(key->pubkey.z);
        err = mp_init(&key->k);
    #else
        err = mp_init_multi(&key->k,
                    key->pubkey.x, key->pubkey.y, key->pubkey.z, NULL, NULL);
    #endif
    if (err != MP_OKAY)
        return MEMORY_E;

     
    pointType = in[0];
    if (pointType != ECC_POINT_UNCOMP && pointType != ECC_POINT_COMP_EVEN &&
                                         pointType != ECC_POINT_COMP_ODD) {
        err = ASN_PARSE_E;
    }

    if (pointType == ECC_POINT_COMP_EVEN || pointType == ECC_POINT_COMP_ODD) {
    #ifdef HAVE_COMP_KEY
        compressed = 1;
    #else
        err = NOT_COMPILED_IN;
    #endif
    }

     
    inLen -= 1;
    in += 1;

    if (err == MP_OKAY) {
    #ifdef HAVE_COMP_KEY
         
        if (compressed)
            inLen = inLen*2 + 1;   
    #endif

         
        keysize = (inLen>>1);
        err = wc_ecc_set_curve(key, keysize, curve_id);
        key->type = ECC_PUBLICKEY;
    }

     
    if (err == MP_OKAY)
        err = mp_read_unsigned_bin(key->pubkey.x, (byte*)in, keysize);

#ifdef HAVE_COMP_KEY
    if (err == MP_OKAY && compressed == 1) {    
#ifndef WOLFSSL_SP_MATH
        mp_int t1, t2;
        int did_init = 0;

        DECLARE_CURVE_SPECS(3)

        if (mp_init_multi(&t1, &t2, NULL, NULL, NULL, NULL) != MP_OKAY)
            err = MEMORY_E;
        else
            did_init = 1;

         
        if (err == MP_OKAY)
            err = wc_ecc_curve_load(key->dp, &curve,
                (ECC_CURVE_FIELD_PRIME | ECC_CURVE_FIELD_AF |
                 ECC_CURVE_FIELD_BF));

         
        if (err == MP_OKAY)
            err = mp_sqr(key->pubkey.x, &t1);
        if (err == MP_OKAY)
            err = mp_mulmod(&t1, key->pubkey.x, curve->prime, &t1);

         
        if (err == MP_OKAY)
            err = mp_mulmod(curve->Af, key->pubkey.x, curve->prime, &t2);
        if (err == MP_OKAY)
            err = mp_add(&t1, &t2, &t1);

         
        if (err == MP_OKAY)
            err = mp_add(&t1, curve->Bf, &t1);

         
        if (err == MP_OKAY)
            err = mp_sqrtmod_prime(&t1, curve->prime, &t2);

         
        if (err == MP_OKAY) {
            if ((mp_isodd(&t2) == MP_YES && pointType == ECC_POINT_COMP_ODD) ||
                (mp_isodd(&t2) == MP_NO &&  pointType == ECC_POINT_COMP_EVEN)) {
                err = mp_mod(&t2, curve->prime, &t2);
            }
            else {
                err = mp_submod(curve->prime, &t2, curve->prime, &t2);
            }
            if (err == MP_OKAY)
                err = mp_copy(&t2, key->pubkey.y);
        }

        if (did_init) {
            mp_clear(&t2);
            mp_clear(&t1);
        }

        wc_ecc_curve_free(curve);
#else
        sp_ecc_uncompress_256(key->pubkey.x, pointType, key->pubkey.y);
#endif
    }
#endif  

    if (err == MP_OKAY && compressed == 0)
        err = mp_read_unsigned_bin(key->pubkey.y, (byte*)in + keysize, keysize);
    if (err == MP_OKAY)
        err = mp_set(key->pubkey.z, 1);

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

    return err;
}