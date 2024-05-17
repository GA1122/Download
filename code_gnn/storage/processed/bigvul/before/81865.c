static int wc_ecc_curve_load_item(const char* src, mp_int** dst,
    ecc_curve_spec* curve, byte mask)
{
    int err;

#ifndef ECC_CACHE_CURVE
     
    if (curve->spec_use >= curve->spec_count) {
        WOLFSSL_MSG("Invalid DECLARE_CURVE_SPECS count");
        return ECC_BAD_ARG_E;
    }
    *dst = &curve->spec_ints[curve->spec_use++];
#endif

    err = mp_init(*dst);
    if (err == MP_OKAY) {
        curve->load_mask |= mask;

        err = mp_read_radix(*dst, src, MP_RADIX_HEX);

    #ifdef HAVE_WOLF_BIGINT
        if (err == MP_OKAY)
            err = wc_mp_to_bigint(*dst, &(*dst)->raw);
    #endif
    }
    return err;
}