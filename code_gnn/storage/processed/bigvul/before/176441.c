WORD32 ih264d_get_vui_params(iv_obj_t *dec_hdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 ih264d_ctl_get_vui_params_ip_t *ps_ip;
 ih264d_ctl_get_vui_params_op_t *ps_op;
 dec_struct_t *ps_dec = dec_hdl->pv_codec_handle;
 dec_seq_params_t *ps_sps;
 vui_t *ps_vui;
    WORD32 i;
    UWORD32 u4_size;

    ps_ip = (ih264d_ctl_get_vui_params_ip_t *)pv_api_ip;
    ps_op = (ih264d_ctl_get_vui_params_op_t *)pv_api_op;
    UNUSED(ps_ip);

    u4_size = ps_op->u4_size;
    memset(ps_op, 0, sizeof(ih264d_ctl_get_vui_params_op_t));
    ps_op->u4_size = u4_size;

 if(NULL == ps_dec->ps_cur_sps)
 {
        ps_op->u4_error_code = ERROR_VUI_PARAMS_NOT_FOUND;
 return IV_FAIL;
 }

    ps_sps = ps_dec->ps_cur_sps;
 if((0 == ps_sps->u1_is_valid)
 || (0 == ps_sps->u1_vui_parameters_present_flag))
 {
        ps_op->u4_error_code = ERROR_VUI_PARAMS_NOT_FOUND;
 return IV_FAIL;
 }

    ps_vui = &ps_sps->s_vui;

    ps_op->u1_aspect_ratio_idc              = ps_vui->u1_aspect_ratio_idc;
    ps_op->u2_sar_width                     = ps_vui->u2_sar_width;
    ps_op->u2_sar_height                    = ps_vui->u2_sar_height;
    ps_op->u1_overscan_appropriate_flag     = ps_vui->u1_overscan_appropriate_flag;
    ps_op->u1_video_format                  = ps_vui->u1_video_format;
    ps_op->u1_video_full_range_flag         = ps_vui->u1_video_full_range_flag;
    ps_op->u1_colour_primaries              = ps_vui->u1_colour_primaries;
    ps_op->u1_tfr_chars                     = ps_vui->u1_tfr_chars;
    ps_op->u1_matrix_coeffs                 = ps_vui->u1_matrix_coeffs;
    ps_op->u1_cr_top_field                  = ps_vui->u1_cr_top_field;
    ps_op->u1_cr_bottom_field               = ps_vui->u1_cr_bottom_field;
    ps_op->u4_num_units_in_tick             = ps_vui->u4_num_units_in_tick;
    ps_op->u4_time_scale                    = ps_vui->u4_time_scale;
    ps_op->u1_fixed_frame_rate_flag         = ps_vui->u1_fixed_frame_rate_flag;
    ps_op->u1_nal_hrd_params_present        = ps_vui->u1_nal_hrd_params_present;
    ps_op->u1_vcl_hrd_params_present        = ps_vui->u1_vcl_hrd_params_present;
    ps_op->u1_low_delay_hrd_flag            = ps_vui->u1_low_delay_hrd_flag;
    ps_op->u1_pic_struct_present_flag       = ps_vui->u1_pic_struct_present_flag;
    ps_op->u1_bitstream_restriction_flag    = ps_vui->u1_bitstream_restriction_flag;
    ps_op->u1_mv_over_pic_boundaries_flag   = ps_vui->u1_mv_over_pic_boundaries_flag;
    ps_op->u4_max_bytes_per_pic_denom       = ps_vui->u4_max_bytes_per_pic_denom;
    ps_op->u4_max_bits_per_mb_denom         = ps_vui->u4_max_bits_per_mb_denom;
    ps_op->u4_log2_max_mv_length_horz       = ps_vui->u4_log2_max_mv_length_horz;
    ps_op->u4_log2_max_mv_length_vert       = ps_vui->u4_log2_max_mv_length_vert;
    ps_op->u4_num_reorder_frames            = ps_vui->u4_num_reorder_frames;
    ps_op->u4_max_dec_frame_buffering       = ps_vui->u4_max_dec_frame_buffering;

 return IV_SUCCESS;
}
