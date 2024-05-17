uint8_t adts_frame(adts_header *adts, bitfile *ld)
{
     
    if (adts_fixed_header(adts, ld))
        return 5;
    adts_variable_header(adts, ld);
    adts_error_check(adts, ld);

    return 0;
}
