static void wc_ecc_curve_free(ecc_curve_spec* curve)
{
     
#ifndef ECC_CACHE_CURVE
    _wc_ecc_curve_free(curve);
#endif
    (void)curve;
}
