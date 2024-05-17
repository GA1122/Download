static uint8_t single_lfe_channel_element(NeAACDecStruct *hDecoder, bitfile *ld,
                                          uint8_t channel, uint8_t *tag)
{
    uint8_t retval = 0;
    element sce = {0};
    ic_stream *ics = &(sce.ics1);
    ALIGN int16_t spec_data[1024] = {0};

    sce.element_instance_tag = (uint8_t)faad_getbits(ld, LEN_TAG
        DEBUGVAR(1,38,"single_lfe_channel_element(): element_instance_tag"));

    *tag = sce.element_instance_tag;
    sce.channel = channel;
    sce.paired_channel = -1;

    retval = individual_channel_stream(hDecoder, &sce, ld, ics, 0, spec_data);
    if (retval > 0)
        return retval;

     
    if (ics->is_used)
        return 32;

#ifdef SBR_DEC
     
     
    if (faad_showbits(ld, LEN_SE_ID) == ID_FIL)
    {
        faad_flushbits(ld, LEN_SE_ID);

         
        if ((retval = fill_element(hDecoder, ld, hDecoder->drc, hDecoder->fr_ch_ele)) > 0)
        {
            return retval;
        }
    }
#endif

     
    retval = reconstruct_single_channel(hDecoder, ics, &sce, spec_data);
    if (retval > 0)
        return retval;

    return 0;
}
