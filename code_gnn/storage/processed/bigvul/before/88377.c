static uint8_t dynamic_range_info(bitfile *ld, drc_info *drc)
{
    uint8_t i, n = 1;
    uint8_t band_incr;

    drc->num_bands = 1;

    if (faad_get1bit(ld
        DEBUGVAR(1,90,"dynamic_range_info(): has instance_tag")) & 1)
    {
        drc->pce_instance_tag = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,91,"dynamic_range_info(): pce_instance_tag"));
          faad_getbits(ld, 4
            DEBUGVAR(1,92,"dynamic_range_info(): drc_tag_reserved_bits"));
        n++;
    }

    drc->excluded_chns_present = faad_get1bit(ld
        DEBUGVAR(1,93,"dynamic_range_info(): excluded_chns_present"));
    if (drc->excluded_chns_present == 1)
    {
        n += excluded_channels(ld, drc);
    }

    if (faad_get1bit(ld
        DEBUGVAR(1,94,"dynamic_range_info(): has bands data")) & 1)
    {
        band_incr = (uint8_t)faad_getbits(ld, 4
            DEBUGVAR(1,95,"dynamic_range_info(): band_incr"));
          faad_getbits(ld, 4
            DEBUGVAR(1,96,"dynamic_range_info(): drc_bands_reserved_bits"));
        n++;
        drc->num_bands += band_incr;

        for (i = 0; i < drc->num_bands; i++)
        {
            drc->band_top[i] = (uint8_t)faad_getbits(ld, 8
                DEBUGVAR(1,97,"dynamic_range_info(): band_top"));
            n++;
        }
    }

    if (faad_get1bit(ld
        DEBUGVAR(1,98,"dynamic_range_info(): has prog_ref_level")) & 1)
    {
        drc->prog_ref_level = (uint8_t)faad_getbits(ld, 7
            DEBUGVAR(1,99,"dynamic_range_info(): prog_ref_level"));
          faad_get1bit(ld
            DEBUGVAR(1,100,"dynamic_range_info(): prog_ref_level_reserved_bits"));
        n++;
    }

    for (i = 0; i < drc->num_bands; i++)
    {
        drc->dyn_rng_sgn[i] = faad_get1bit(ld
            DEBUGVAR(1,101,"dynamic_range_info(): dyn_rng_sgn"));
        drc->dyn_rng_ctl[i] = (uint8_t)faad_getbits(ld, 7
            DEBUGVAR(1,102,"dynamic_range_info(): dyn_rng_ctl"));
        n++;
    }

    return n;
}
