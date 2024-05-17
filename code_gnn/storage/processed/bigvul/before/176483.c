WORD32 ihevcd_get_vui_params(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
 ihevcd_cxa_ctl_get_vui_params_ip_t *ps_ip;
 ihevcd_cxa_ctl_get_vui_params_op_t *ps_op;
 codec_t *ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;
 sps_t *ps_sps;
 vui_t *ps_vui;
    WORD32 i;

    ps_ip = (ihevcd_cxa_ctl_get_vui_params_ip_t *)pv_api_ip;
    ps_op = (ihevcd_cxa_ctl_get_vui_params_op_t *)pv_api_op;

 if(0 == ps_codec->i4_sps_done)
 {
        ps_op->u4_error_code = IHEVCD_VUI_PARAMS_NOT_FOUND;
 return IV_FAIL;
 }

    ps_sps = ps_codec->s_parse.ps_sps;
 if(0 == ps_sps->i1_sps_valid || 0 == ps_sps->i1_vui_parameters_present_flag)
 {
        WORD32 sps_idx = 0;
        ps_sps = ps_codec->ps_sps_base;

 while((0 == ps_sps->i1_sps_valid) || (0 == ps_sps->i1_vui_parameters_present_flag))
 {
            sps_idx++;
            ps_sps++;

 if(sps_idx == MAX_SPS_CNT - 1)
 {
                ps_op->u4_error_code = IHEVCD_VUI_PARAMS_NOT_FOUND;
 return IV_FAIL;
 }
 }
 }

    ps_vui = &ps_sps->s_vui_parameters;
    UNUSED(ps_ip);

    ps_op->u1_aspect_ratio_info_present_flag         =  ps_vui->u1_aspect_ratio_info_present_flag;
    ps_op->u1_aspect_ratio_idc                       =  ps_vui->u1_aspect_ratio_idc;
    ps_op->u2_sar_width                              =  ps_vui->u2_sar_width;
    ps_op->u2_sar_height                             =  ps_vui->u2_sar_height;
    ps_op->u1_overscan_info_present_flag             =  ps_vui->u1_overscan_info_present_flag;
    ps_op->u1_overscan_appropriate_flag              =  ps_vui->u1_overscan_appropriate_flag;
    ps_op->u1_video_signal_type_present_flag         =  ps_vui->u1_video_signal_type_present_flag;
    ps_op->u1_video_format                           =  ps_vui->u1_video_format;
    ps_op->u1_video_full_range_flag                  =  ps_vui->u1_video_full_range_flag;
    ps_op->u1_colour_description_present_flag        =  ps_vui->u1_colour_description_present_flag;
    ps_op->u1_colour_primaries                       =  ps_vui->u1_colour_primaries;
    ps_op->u1_transfer_characteristics               =  ps_vui->u1_transfer_characteristics;
    ps_op->u1_matrix_coefficients                    =  ps_vui->u1_matrix_coefficients;
    ps_op->u1_chroma_loc_info_present_flag           =  ps_vui->u1_chroma_loc_info_present_flag;
    ps_op->u1_chroma_sample_loc_type_top_field       =  ps_vui->u1_chroma_sample_loc_type_top_field;
    ps_op->u1_chroma_sample_loc_type_bottom_field    =  ps_vui->u1_chroma_sample_loc_type_bottom_field;
    ps_op->u1_neutral_chroma_indication_flag         =  ps_vui->u1_neutral_chroma_indication_flag;
    ps_op->u1_field_seq_flag                         =  ps_vui->u1_field_seq_flag;
    ps_op->u1_frame_field_info_present_flag          =  ps_vui->u1_frame_field_info_present_flag;
    ps_op->u1_default_display_window_flag            =  ps_vui->u1_default_display_window_flag;
    ps_op->u4_def_disp_win_left_offset               =  ps_vui->u4_def_disp_win_left_offset;
    ps_op->u4_def_disp_win_right_offset              =  ps_vui->u4_def_disp_win_right_offset;
    ps_op->u4_def_disp_win_top_offset                =  ps_vui->u4_def_disp_win_top_offset;
    ps_op->u4_def_disp_win_bottom_offset             =  ps_vui->u4_def_disp_win_bottom_offset;
    ps_op->u1_vui_hrd_parameters_present_flag        =  ps_vui->u1_vui_hrd_parameters_present_flag;
    ps_op->u1_vui_timing_info_present_flag           =  ps_vui->u1_vui_timing_info_present_flag;
    ps_op->u4_vui_num_units_in_tick                  =  ps_vui->u4_vui_num_units_in_tick;
    ps_op->u4_vui_time_scale                         =  ps_vui->u4_vui_time_scale;
    ps_op->u1_poc_proportional_to_timing_flag        =  ps_vui->u1_poc_proportional_to_timing_flag;
    ps_op->u1_num_ticks_poc_diff_one_minus1          =  ps_vui->u1_num_ticks_poc_diff_one_minus1;
    ps_op->u1_bitstream_restriction_flag             =  ps_vui->u1_bitstream_restriction_flag;
    ps_op->u1_tiles_fixed_structure_flag             =  ps_vui->u1_tiles_fixed_structure_flag;
    ps_op->u1_motion_vectors_over_pic_boundaries_flag =  ps_vui->u1_motion_vectors_over_pic_boundaries_flag;
    ps_op->u1_restricted_ref_pic_lists_flag          =  ps_vui->u1_restricted_ref_pic_lists_flag;
    ps_op->u4_min_spatial_segmentation_idc           =  ps_vui->u4_min_spatial_segmentation_idc;
    ps_op->u1_max_bytes_per_pic_denom                =  ps_vui->u1_max_bytes_per_pic_denom;
    ps_op->u1_max_bits_per_mincu_denom               =  ps_vui->u1_max_bits_per_mincu_denom;
    ps_op->u1_log2_max_mv_length_horizontal          =  ps_vui->u1_log2_max_mv_length_horizontal;
    ps_op->u1_log2_max_mv_length_vertical            =  ps_vui->u1_log2_max_mv_length_vertical;


  
    ps_op->u1_timing_info_present_flag                         =    ps_vui->s_vui_hrd_parameters.u1_timing_info_present_flag;
    ps_op->u4_num_units_in_tick                                =    ps_vui->s_vui_hrd_parameters.u4_num_units_in_tick;
    ps_op->u4_time_scale                                       =    ps_vui->s_vui_hrd_parameters.u4_time_scale;
    ps_op->u1_nal_hrd_parameters_present_flag                  =    ps_vui->s_vui_hrd_parameters.u1_nal_hrd_parameters_present_flag;
    ps_op->u1_vcl_hrd_parameters_present_flag                  =    ps_vui->s_vui_hrd_parameters.u1_vcl_hrd_parameters_present_flag;
    ps_op->u1_cpbdpb_delays_present_flag                       =    ps_vui->s_vui_hrd_parameters.u1_cpbdpb_delays_present_flag;
    ps_op->u1_sub_pic_cpb_params_present_flag                  =    ps_vui->s_vui_hrd_parameters.u1_sub_pic_cpb_params_present_flag;
    ps_op->u1_tick_divisor_minus2                              =    ps_vui->s_vui_hrd_parameters.u1_tick_divisor_minus2;
    ps_op->u1_du_cpb_removal_delay_increment_length_minus1     =    ps_vui->s_vui_hrd_parameters.u1_du_cpb_removal_delay_increment_length_minus1;
    ps_op->u1_sub_pic_cpb_params_in_pic_timing_sei_flag        =    ps_vui->s_vui_hrd_parameters.u1_sub_pic_cpb_params_in_pic_timing_sei_flag;
    ps_op->u1_dpb_output_delay_du_length_minus1                =    ps_vui->s_vui_hrd_parameters.u1_dpb_output_delay_du_length_minus1;
    ps_op->u4_bit_rate_scale                                   =    ps_vui->s_vui_hrd_parameters.u4_bit_rate_scale;
    ps_op->u4_cpb_size_scale                                   =    ps_vui->s_vui_hrd_parameters.u4_cpb_size_scale;
    ps_op->u4_cpb_size_du_scale                                =    ps_vui->s_vui_hrd_parameters.u4_cpb_size_du_scale;
    ps_op->u1_initial_cpb_removal_delay_length_minus1          =    ps_vui->s_vui_hrd_parameters.u1_initial_cpb_removal_delay_length_minus1;
    ps_op->u1_au_cpb_removal_delay_length_minus1               =    ps_vui->s_vui_hrd_parameters.u1_au_cpb_removal_delay_length_minus1;
    ps_op->u1_dpb_output_delay_length_minus1                   =    ps_vui->s_vui_hrd_parameters.u1_dpb_output_delay_length_minus1;

 for(i = 0; i < 6; i++)
 {
        ps_op->au1_fixed_pic_rate_general_flag[i] =    ps_vui->s_vui_hrd_parameters.au1_fixed_pic_rate_general_flag[i];
        ps_op->au1_fixed_pic_rate_within_cvs_flag[i] =    ps_vui->s_vui_hrd_parameters.au1_fixed_pic_rate_within_cvs_flag[i];
        ps_op->au1_elemental_duration_in_tc_minus1[i] =    ps_vui->s_vui_hrd_parameters.au1_elemental_duration_in_tc_minus1[i];
        ps_op->au1_low_delay_hrd_flag[i] =    ps_vui->s_vui_hrd_parameters.au1_low_delay_hrd_flag[i];
        ps_op->au1_cpb_cnt_minus1[i] =    ps_vui->s_vui_hrd_parameters.au1_cpb_cnt_minus1[i];
 }


 return IV_SUCCESS;
}
