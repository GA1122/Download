void raw_data_block(NeAACDecStruct *hDecoder, NeAACDecFrameInfo *hInfo,
                    bitfile *ld, program_config *pce, drc_info *drc)
{
    uint8_t id_syn_ele;
    uint8_t ele_this_frame = 0;

    hDecoder->fr_channels = 0;
    hDecoder->fr_ch_ele = 0;
    hDecoder->first_syn_ele = 25;
    hDecoder->has_lfe = 0;

#ifdef ERROR_RESILIENCE
    if (hDecoder->object_type < ER_OBJECT_START)
    {
#endif
         
        while ((id_syn_ele = (uint8_t)faad_getbits(ld, LEN_SE_ID
            DEBUGVAR(1,4,"NeAACDecDecode(): id_syn_ele"))) != ID_END)
        {
            switch (id_syn_ele) {
            case ID_SCE:
                ele_this_frame++;
                if (hDecoder->first_syn_ele == 25) hDecoder->first_syn_ele = id_syn_ele;
                decode_sce_lfe(hDecoder, hInfo, ld, id_syn_ele);
                if (hInfo->error > 0)
                    return;
                break;
            case ID_CPE:
                ele_this_frame++;
                if (hDecoder->first_syn_ele == 25) hDecoder->first_syn_ele = id_syn_ele;
                decode_cpe(hDecoder, hInfo, ld, id_syn_ele);
                if (hInfo->error > 0)
                    return;
                break;
            case ID_LFE:
#ifdef DRM
                hInfo->error = 32;
#else
                ele_this_frame++;
                hDecoder->has_lfe++;
                decode_sce_lfe(hDecoder, hInfo, ld, id_syn_ele);
#endif
                if (hInfo->error > 0)
                    return;
                break;
            case ID_CCE:  
#ifdef DRM
                hInfo->error = 32;
#else
                ele_this_frame++;
#ifdef COUPLING_DEC
                hInfo->error = coupling_channel_element(hDecoder, ld);
#else
                hInfo->error = 6;
#endif
#endif
                if (hInfo->error > 0)
                    return;
                break;
            case ID_DSE:
                ele_this_frame++;
                data_stream_element(hDecoder, ld);
                break;
            case ID_PCE:
                if (ele_this_frame != 0)
                {
                    hInfo->error = 31;
                    return;
                }
                ele_this_frame++;
                 
                 
                program_config_element(pce, ld);
                break;
            case ID_FIL:
                ele_this_frame++;
                 
                 
                 
                if ((hInfo->error = fill_element(hDecoder, ld, drc
#ifdef SBR_DEC
                    , INVALID_SBR_ELEMENT
#endif
                    )) > 0)
                    return;
                break;
            }
        }
#ifdef ERROR_RESILIENCE
    } else {
         
        switch (hDecoder->channelConfiguration)
        {
        case 1:
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            if (hInfo->error > 0)
                return;
            break;
        case 2:
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            if (hInfo->error > 0)
                return;
            break;
        case 3:
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            if (hInfo->error > 0)
                return;
            break;
        case 4:
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            if (hInfo->error > 0)
                return;
            break;
        case 5:
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            if (hInfo->error > 0)
                return;
            break;
        case 6:
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_sce_lfe(hDecoder, hInfo, ld, ID_LFE);
            if (hInfo->error > 0)
                return;
            break;
        case 7:  
            decode_sce_lfe(hDecoder, hInfo, ld, ID_SCE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_cpe(hDecoder, hInfo, ld, ID_CPE);
            decode_sce_lfe(hDecoder, hInfo, ld, ID_LFE);
            if (hInfo->error > 0)
                return;
            break;
        default:
            hInfo->error = 7;
            return;
        }
#if 0
        cnt = bits_to_decode() / 8;
        while (cnt >= 1)
        {
            cnt -= extension_payload(cnt);
        }
#endif
    }
#endif

     
#ifdef DRM
    if (hDecoder->object_type != DRM_ER_LC
#if 0
        && !hDecoder->latm_header_present
#endif
        )
#endif
    {
        faad_byte_align(ld);
    }

    return;
}
