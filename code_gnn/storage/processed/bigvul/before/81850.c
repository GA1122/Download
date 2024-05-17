int wc_ecc_check_key(ecc_key* key)
{
    int    err;
#ifndef WOLFSSL_SP_MATH
#ifndef WOLFSSL_ATECC508A
    mp_int* b;
#ifdef USE_ECC_B_PARAM
    DECLARE_CURVE_SPECS(4)
#else
    mp_int b_lcl;
    DECLARE_CURVE_SPECS(3)
    b = &b_lcl;
    XMEMSET(b, 0, sizeof(mp_int));
#endif
#endif  

    if (key == NULL)
        return BAD_FUNC_ARG;

#ifdef WOLFSSL_ATECC508A

    if (key->slot == ATECC_INVALID_SLOT)
        return ECC_BAD_ARG_E;

    err = 0;  

#else

     
    if (wc_ecc_point_is_at_infinity(&key->pubkey))
        return ECC_INF_E;

     
    err = wc_ecc_curve_load(key->dp, &curve, (ECC_CURVE_FIELD_PRIME |
            ECC_CURVE_FIELD_AF | ECC_CURVE_FIELD_ORDER
#ifdef USE_ECC_B_PARAM
            | ECC_CURVE_FIELD_BF
#endif
    ));

#ifndef USE_ECC_B_PARAM
     
    if (err == MP_OKAY)
        err = mp_init(b);
    if (err == MP_OKAY)
        err = mp_read_radix(b, key->dp->Bf, MP_RADIX_HEX);
#else
    b = curve->Bf;
#endif

     
    if (mp_cmp(key->pubkey.x, curve->prime) != MP_LT)
        err = ECC_OUT_OF_RANGE_E;

     
    if (mp_cmp(key->pubkey.y, curve->prime) != MP_LT)
        err = ECC_OUT_OF_RANGE_E;

     
    if (err == MP_OKAY)
        err = wc_ecc_is_point(&key->pubkey, curve->Af, b, curve->prime);

     
    if (err == MP_OKAY)
        err = ecc_check_pubkey_order(key, &key->pubkey, curve->Af, curve->prime,
                curve->order);

     
    if (err == MP_OKAY && key->type == ECC_PRIVATEKEY)
        err = ecc_check_privkey_gen(key, curve->Af, curve->prime);

    wc_ecc_curve_free(curve);

#ifndef USE_ECC_B_PARAM
    mp_clear(b);
#endif

#endif  
#else
    if (key == NULL)
        return BAD_FUNC_ARG;

     
    if (key->idx != ECC_CUSTOM_IDX && ecc_sets[key->idx].id == ECC_SECP256R1) {
        err = sp_ecc_check_key_256(key->pubkey.x, key->pubkey.y, &key->k,
                                                                     key->heap);
    }
    else
        err = WC_KEY_SIZE_E;
#endif

    return err;
}
