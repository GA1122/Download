static uint8_t scale_factor_data(NeAACDecStruct *hDecoder, ic_stream *ics, bitfile *ld)
{
    uint8_t ret = 0;
#ifdef PROFILE
    int64_t count = faad_get_ts();
#endif

#ifdef ERROR_RESILIENCE
    if (!hDecoder->aacScalefactorDataResilienceFlag)
    {
#endif
        ret = decode_scale_factors(ics, ld);
#ifdef ERROR_RESILIENCE
    } else {
         
        ret = rvlc_scale_factor_data(ics, ld);
    }
#endif

#ifdef PROFILE
    count = faad_get_ts() - count;
    hDecoder->scalefac_cycles += count;
#endif

    return ret;
}
