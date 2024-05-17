int wc_ecc_rs_raw_to_sig(const byte* r, word32 rSz, const byte* s, word32 sSz,
    byte* out, word32* outlen)
{
    int err;
    mp_int rtmp;
    mp_int stmp;

    if (r == NULL || s == NULL || out == NULL || outlen == NULL)
        return ECC_BAD_ARG_E;

    err = mp_init_multi(&rtmp, &stmp, NULL, NULL, NULL, NULL);
    if (err != MP_OKAY)
        return err;

    err = mp_read_unsigned_bin(&rtmp, r, rSz);
    if (err == MP_OKAY)
        err = mp_read_unsigned_bin(&stmp, s, sSz);

     
    if (err == MP_OKAY)
        err = StoreECC_DSA_Sig(out, outlen, &rtmp, &stmp);

    if (err == MP_OKAY) {
        if (mp_iszero(&rtmp) == MP_YES || mp_iszero(&stmp) == MP_YES)
            err = MP_ZERO_E;
    }

    mp_clear(&rtmp);
    mp_clear(&stmp);

    return err;
}