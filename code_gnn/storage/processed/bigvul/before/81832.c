static int ecc_check_privkey_gen(ecc_key* key, mp_int* a, mp_int* prime)
{
    int        err = MP_OKAY;
    ecc_point* base = NULL;
    ecc_point* res  = NULL;
    DECLARE_CURVE_SPECS(2)

    if (key == NULL)
        return BAD_FUNC_ARG;

    res = wc_ecc_new_point_h(key->heap);
    if (res == NULL)
        err = MEMORY_E;

#ifdef WOLFSSL_HAVE_SP_ECC
#ifndef WOLFSSL_SP_NO_256
    if (key->idx != ECC_CUSTOM_IDX && ecc_sets[key->idx].id == ECC_SECP256R1) {
        if (err == MP_OKAY)
            err = sp_ecc_mulmod_base_256(&key->k, res, 1, key->heap);
    }
    else
#endif
#endif
    {
        base = wc_ecc_new_point_h(key->heap);
        if (base == NULL)
            err = MEMORY_E;

        if (err == MP_OKAY) {
             
            err = wc_ecc_curve_load(key->dp, &curve,
                                      (ECC_CURVE_FIELD_GX | ECC_CURVE_FIELD_GY));
        }

         
        if (err == MP_OKAY)
            err = mp_copy(curve->Gx, base->x);
        if (err == MP_OKAY)
            err = mp_copy(curve->Gy, base->y);
        if (err == MP_OKAY)
            err = mp_set(base->z, 1);

        if (err == MP_OKAY)
            err = wc_ecc_mulmod_ex(&key->k, base, res, a, prime, 1, key->heap);
    }

    if (err == MP_OKAY) {
         
        if (mp_cmp(res->x, key->pubkey.x) != MP_EQ ||
            mp_cmp(res->y, key->pubkey.y) != MP_EQ ||
            mp_cmp(res->z, key->pubkey.z) != MP_EQ) {
             
            err = ECC_PRIV_KEY_E;
        }
    }

    wc_ecc_curve_free(curve);
    wc_ecc_del_point_h(res, key->heap);
    wc_ecc_del_point_h(base, key->heap);

    return err;
}