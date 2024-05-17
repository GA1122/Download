IV_API_CALL_STATUS_T impeg2d_api_get_seq_info(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 impeg2d_ctl_get_seq_info_ip_t *ps_ip;
 impeg2d_ctl_get_seq_info_op_t *ps_op;
 dec_state_t *ps_codec;
 dec_state_multi_core_t *ps_dec_state_multi_core;

    ps_ip  = (impeg2d_ctl_get_seq_info_ip_t *)pv_api_ip;
    ps_op = (impeg2d_ctl_get_seq_info_op_t *)pv_api_op;

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_codec = ps_dec_state_multi_core->ps_dec_state[0];
    UNUSED(ps_ip);
 if(ps_codec->u2_header_done == 1)
 {
        ps_op->u1_aspect_ratio_information = ps_codec->u2_aspect_ratio_info;
        ps_op->u1_frame_rate_code = ps_codec->u2_frame_rate_code;
        ps_op->u1_frame_rate_extension_n = ps_codec->u2_frame_rate_extension_n;
        ps_op->u1_frame_rate_extension_d = ps_codec->u2_frame_rate_extension_d;
 if(ps_codec->u1_seq_disp_extn_present == 1)
 {
            ps_op->u1_video_format = ps_codec->u1_video_format;
            ps_op->u1_colour_primaries = ps_codec->u1_colour_primaries;
            ps_op->u1_transfer_characteristics = ps_codec->u1_transfer_characteristics;
            ps_op->u1_matrix_coefficients = ps_codec->u1_matrix_coefficients;
            ps_op->u2_display_horizontal_size = ps_codec->u2_display_horizontal_size;
            ps_op->u2_display_vertical_size = ps_codec->u2_display_vertical_size;
 }
 else
 {
            ps_op->u1_video_format = 5;
            ps_op->u1_colour_primaries = 2;
            ps_op->u1_transfer_characteristics = 2;
            ps_op->u1_matrix_coefficients = 2;
            ps_op->u2_display_horizontal_size = ps_codec->u2_horizontal_size;
            ps_op->u2_display_vertical_size = ps_codec->u2_vertical_size;
 }
        ps_op->u4_error_code = IV_SUCCESS;
 return IV_SUCCESS;
 }
 else
 {
        ps_op->u4_error_code = IV_FAIL;
 return IV_FAIL;
 }
}
