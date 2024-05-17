static void decode_sce_lfe(NeAACDecStruct *hDecoder,
                           NeAACDecFrameInfo *hInfo, bitfile *ld,
                           uint8_t id_syn_ele)
{
    uint8_t channels = hDecoder->fr_channels;
    uint8_t tag = 0;

    if (channels+1 > MAX_CHANNELS)
    {
        hInfo->error = 12;
        return;
    }
    if (hDecoder->fr_ch_ele+1 > MAX_SYNTAX_ELEMENTS)
    {
        hInfo->error = 13;
        return;
    }

     

    if (hDecoder->frame && hDecoder->element_id[hDecoder->fr_ch_ele] != id_syn_ele) {
         
        hInfo->error = 21;
        return;
    }

     
    hDecoder->element_id[hDecoder->fr_ch_ele] = id_syn_ele;

     
    hInfo->error = single_lfe_channel_element(hDecoder, ld, channels, &tag);

     
    if (hDecoder->element_output_channels[hDecoder->fr_ch_ele] == 2)
    {
         
        hDecoder->internal_channel[channels] = channels;
        hDecoder->internal_channel[channels+1] = channels+1;
    } else {
        if (hDecoder->pce_set)
            hDecoder->internal_channel[hDecoder->pce.sce_channel[tag]] = channels;
        else
            hDecoder->internal_channel[channels] = channels;
    }

    hDecoder->fr_channels += hDecoder->element_output_channels[hDecoder->fr_ch_ele];
    hDecoder->fr_ch_ele++;
}
