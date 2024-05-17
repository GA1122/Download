int wc_ecc_import_point_der(byte* in, word32 inLen, const int curve_idx,
                            ecc_point* point)
{
    int err = 0;
    int compressed = 0;
    int keysize;
    byte pointType;

    if (in == NULL || point == NULL || (curve_idx < 0) ||
        (wc_ecc_is_valid_idx(curve_idx) == 0))
        return ECC_BAD_ARG_E;

     
    if ((inLen & 1) == 0) {
        return ECC_BAD_ARG_E;
    }

     
#ifdef ALT_ECC_SIZE
    point->x = (mp_int*)&point->xyz[0];
    point->y = (mp_int*)&point->xyz[1];
    point->z = (mp_int*)&point->xyz[2];
    alt_fp_init(point->x);
    alt_fp_init(point->y);
    alt_fp_init(point->z);
#else
    err = mp_init_multi(point->x, point->y, point->z, NULL, NULL, NULL);
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

     
    keysize = inLen>>1;

#ifdef WOLFSSL_ATECC508A
     
    XMEMCPY(key->pubkey_raw, (byte*)in, sizeof(key->pubkey_raw));
#endif

     
    if (err == MP_OKAY)
        err = mp_read_unsigned_bin(point->x, (byte*)in, keysize);

#ifdef HAVE_COMP_KEY
    if (err == MP_OKAY && compressed == 1) {    
#ifndef WOLFSSL_SP_MATH
        int did_init = 0;
        mp_int t1, t2;
        DECLARE_CURVE_SPECS(3)

        if (mp_init_multi(&t1, &t2, NULL, NULL, NULL, NULL) != MP_OKAY)
            err = MEMORY_E;
        else
            did_init = 1;

         
        if (err == MP_OKAY)
            err = wc_ecc_curve_load(&ecc_sets[curve_idx], &curve,
                (ECC_CURVE_FIELD_PRIME | ECC_CURVE_FIELD_AF |
                    ECC_CURVE_FIELD_BF));

         
        if (err == MP_OKAY)
            err = mp_sqr(point->x, &t1);
        if (err == MP_OKAY)
            err = mp_mulmod(&t1, point->x, curve->prime, &t1);

         
        if (err == MP_OKAY)
            err = mp_mulmod(curve->Af, point->x, curve->prime, &t2);
        if (err == MP_OKAY)
            err = mp_add(&t1, &t2, &t1);

         
        if (err == MP_OKAY)
            err = mp_add(&t1, curve->Bf, &t1);

         
        if (err == MP_OKAY)
            err = mp_sqrtmod_prime(&t1, curve->prime, &t2);

         
        if (err == MP_OKAY) {
            if ((mp_isodd(&t2) == MP_YES && pointType == ECC_POINT_COMP_ODD) ||
                (mp_isodd(&t2) == MP_NO &&  pointType == ECC_POINT_COMP_EVEN)) {
                err = mp_mod(&t2, curve->prime, point->y);
            }
            else {
                err = mp_submod(curve->prime, &t2, curve->prime, point->y);
            }
        }

        if (did_init) {
            mp_clear(&t2);
            mp_clear(&t1);
        }

        wc_ecc_curve_free(curve);
#else
        sp_ecc_uncompress_256(point->x, pointType, point->y);
#endif
    }
#endif

    if (err == MP_OKAY && compressed == 0)
        err = mp_read_unsigned_bin(point->y, (byte*)in + keysize, keysize);
    if (err == MP_OKAY)
        err = mp_set(point->z, 1);

    if (err != MP_OKAY) {
        mp_clear(point->x);
        mp_clear(point->y);
        mp_clear(point->z);
    }

    return err;
}