static int ecc_check_privkey_gen_helper(ecc_key* key)
{
    int    err;
#ifndef WOLFSSL_ATECC508A
    DECLARE_CURVE_SPECS(2)
#endif

    if (key == NULL)
        return BAD_FUNC_ARG;

#ifdef WOLFSSL_ATECC508A
     
    err = BAD_COND_E;

#else

     
    err = wc_ecc_curve_load(key->dp, &curve,
        (ECC_CURVE_FIELD_PRIME | ECC_CURVE_FIELD_AF));

    if (err == MP_OKAY)
        err = ecc_check_privkey_gen(key, curve->Af, curve->prime);

    wc_ecc_curve_free(curve);

#endif  

    return err;
}