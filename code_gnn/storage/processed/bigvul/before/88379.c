uint32_t faad_latm_frame(latm_header *latm, bitfile *ld)
{
    uint16_t len;
    uint32_t initpos, endpos, firstpos, ret;

    firstpos = faad_get_processed_bits(ld);
    while (ld->bytes_left)
    {
        faad_byte_align(ld);
        if(faad_showbits(ld, 11) != 0x2B7)
        {
            faad_getbits(ld, 8);
            continue;
        }
        faad_getbits(ld, 11);
        len = faad_getbits(ld, 13);
        if(!len)
            continue;
        initpos = faad_get_processed_bits(ld);
        ret = latmAudioMuxElement(latm, ld);
        endpos = faad_get_processed_bits(ld);
        if(ret>0)
            return (len*8)-(endpos-initpos);
    }
    return -1U;
}
