void wc_ecc_curve_cache_free(void)
{
    int x;

     
    for (x = 0; x < (int)ECC_SET_COUNT; x++) {
        if (ecc_curve_spec_cache[x]) {
            _wc_ecc_curve_free(ecc_curve_spec_cache[x]);
            XFREE(ecc_curve_spec_cache[x], NULL, DYNAMIC_TYPE_ECC);
            ecc_curve_spec_cache[x] = NULL;
        }
    }

#if defined(ECC_CACHE_CURVE) && !defined(SINGLE_THREADED)
    wc_FreeMutex(&ecc_curve_cache_mutex);
#endif
}