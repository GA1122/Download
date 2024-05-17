int wc_ecc_rs_to_sig(const char* r, const char* s, byte* out, word32* outlen)
{
    int err;
    mp_int rtmp;
    mp_int stmp;

    if (r == NULL || s == NULL || out == NULL || outlen == NULL)
        return ECC_BAD_ARG_E;

    err = mp_init_multi(&rtmp, &stmp, NULL, NULL, NULL, NULL);
    if (err != MP_OKAY)
        return err;

    err = mp_read_radix(&rtmp, r, MP_RADIX_HEX);
    if (err == MP_OKAY)
        err = mp_read_radix(&stmp, s, MP_RADIX_HEX);

     
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