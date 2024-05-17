static void decode_cpe(NeAACDecStruct *hDecoder, NeAACDecFrameInfo *hInfo, bitfile *ld,
                       uint8_t id_syn_ele)
{
    uint8_t channels = hDecoder->fr_channels;
    uint8_t tag = 0;

    if (channels+2 > MAX_CHANNELS)
    {
        hInfo->error = 12;
        return;
    }
    if (hDecoder->fr_ch_ele+1 > MAX_SYNTAX_ELEMENTS)
    {
        hInfo->error = 13;
        return;
    }

     
    if (hDecoder->element_output_channels[hDecoder->fr_ch_ele] == 0)
    {
         
        hDecoder->element_output_channels[hDecoder->fr_ch_ele] = 2;
    } else if (hDecoder->element_output_channels[hDecoder->fr_ch_ele] != 2) {
         
        hInfo->error = 21;
        return;
    }

    if (hDecoder->frame && hDecoder->element_id[hDecoder->fr_ch_ele] != id_syn_ele) {
         
        hInfo->error = 21;
        return;
    }

     
    hDecoder->element_id[hDecoder->fr_ch_ele] = id_syn_ele;

     
    hInfo->error = channel_pair_element(hDecoder, ld, channels, &tag);

     
    if (hDecoder->pce_set)
    {
        hDecoder->internal_channel[hDecoder->pce.cpe_channel[tag]] = channels;
        hDecoder->internal_channel[hDecoder->pce.cpe_channel[tag]+1] = channels+1;
    } else {
        hDecoder->internal_channel[channels] = channels;
        hDecoder->internal_channel[channels+1] = channels+1;
    }

    hDecoder->fr_channels += 2;
    hDecoder->fr_ch_ele++;
}