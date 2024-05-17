IHEVCD_ERROR_T ihevcd_parse_vps(codec_t *ps_codec)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 i;
    WORD32 value;
    WORD32 vps_id;
 vps_t *ps_vps;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
    BITS_PARSE("vps_video_parameter_set_id", value, ps_bitstrm, 4);
    vps_id = value;

 if(vps_id >= MAX_VPS_CNT)
 {
        ps_codec->s_parse.i4_error_code = IHEVCD_UNSUPPORTED_VPS_ID;
 return IHEVCD_UNSUPPORTED_VPS_ID;
 }


    ps_vps = (ps_codec->s_parse.ps_vps_base + vps_id);

    ps_vps->i1_vps_id = vps_id;

    BITS_PARSE("vps_reserved_three_2bits", value, ps_bitstrm, 2);
    ASSERT(value == 3);

    BITS_PARSE("vps_max_layers_minus1", value, ps_bitstrm, 6);



    BITS_PARSE("vps_max_sub_layers_minus1", value, ps_bitstrm, 3);
    ps_vps->i1_vps_max_sub_layers = value + 1;

    ASSERT(ps_vps->i1_vps_max_sub_layers < VPS_MAX_SUB_LAYERS);

    BITS_PARSE("vps_temporal_id_nesting_flag", value, ps_bitstrm, 1);
    ps_vps->i1_vps_temporal_id_nesting_flag = value;

    BITS_PARSE("vps_reserved_ffff_16bits", value, ps_bitstrm, 16);
    ASSERT(value == 0xFFFF);
    ret = ihevcd_profile_tier_level(ps_bitstrm, &(ps_vps->s_ptl),
 1, (ps_vps->i1_vps_max_sub_layers - 1));

    BITS_PARSE("vps_sub_layer_ordering_info_present_flag", value, ps_bitstrm, 1);
    ps_vps->i1_sub_layer_ordering_info_present_flag = value;
    i = (ps_vps->i1_sub_layer_ordering_info_present_flag ?
 0 : (ps_vps->i1_vps_max_sub_layers - 1));
 for(; i < ps_vps->i1_vps_max_sub_layers; i++)
 {
        UEV_PARSE("vps_max_dec_pic_buffering[i]", value, ps_bitstrm);
        ps_vps->ai1_vps_max_dec_pic_buffering[i] = value;

  
        UEV_PARSE("vps_num_reorder_pics[i]", value, ps_bitstrm);
        ps_vps->ai1_vps_max_num_reorder_pics[i] = value;

        UEV_PARSE("vps_max_latency_increase[i]", value, ps_bitstrm);
        ps_vps->ai1_vps_max_latency_increase[i] = value;
 }



    BITS_PARSE("vps_max_layer_id", value, ps_bitstrm, 6);

    UEV_PARSE("vps_num_layer_sets_minus1", value, ps_bitstrm);

    BITS_PARSE("vps_timing_info_present_flag", value, ps_bitstrm, 1);



 return ret;
}
