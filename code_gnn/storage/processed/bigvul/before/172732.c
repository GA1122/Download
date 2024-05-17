WORD32 ih264d_parse_sps(dec_struct_t *ps_dec, dec_bit_stream_t *ps_bitstrm)
{
    UWORD8 i;
 dec_seq_params_t *ps_seq = NULL;
    UWORD8 u1_profile_idc, u1_level_idc, u1_seq_parameter_set_id;
    UWORD16 i2_max_frm_num;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
    UWORD8 u1_frm, uc_constraint_set0_flag, uc_constraint_set1_flag;

    UWORD32 u4_temp;
    WORD32 pic_height_in_map_units_minus1 = 0;
    UWORD32 u2_pic_wd = 0;
    UWORD32 u2_pic_ht = 0;
    UWORD32 u2_frm_wd_y = 0;
    UWORD32 u2_frm_ht_y = 0;
    UWORD32 u2_frm_wd_uv = 0;
    UWORD32 u2_frm_ht_uv = 0;
    UWORD32 u2_crop_offset_y = 0;
    UWORD32 u2_crop_offset_uv = 0;
    WORD32 ret;

  
    WORD32 i4_i;
  
    UWORD8 u1_frame_cropping_flag, u1_frame_cropping_rect_left_ofst,
                    u1_frame_cropping_rect_right_ofst,
                    u1_frame_cropping_rect_top_ofst,
                    u1_frame_cropping_rect_bottom_ofst;
  
  
  
  
    SWITCHONTRACE;
    u1_profile_idc = ih264d_get_bits_h264(ps_bitstrm, 8);
    COPYTHECONTEXT("SPS: profile_idc",u1_profile_idc);

  
    uc_constraint_set0_flag = ih264d_get_bit_h264(ps_bitstrm);
    uc_constraint_set1_flag = ih264d_get_bit_h264(ps_bitstrm);
    ih264d_get_bit_h264(ps_bitstrm);

  
  
  
  
    ih264d_get_bits_h264(ps_bitstrm, 5);
  

  
  
 if((u1_profile_idc != MAIN_PROFILE_IDC) &&

 (u1_profile_idc != BASE_PROFILE_IDC) &&

 (u1_profile_idc != HIGH_PROFILE_IDC)

 )
 {

 if((uc_constraint_set1_flag != 1) && (uc_constraint_set0_flag != 1))
 {
 if(NULL != ps_dec)
 {
                UWORD32 i4_error_code;
                i4_error_code = ERROR_FEATURE_UNAVAIL;
 return i4_error_code;
 }
 else
 {
 return (ERROR_FEATURE_UNAVAIL);
 }
 }
 }

    u1_level_idc = ih264d_get_bits_h264(ps_bitstrm, 8);



    COPYTHECONTEXT("SPS: u4_level_idc",u1_level_idc);

    u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp & MASK_ERR_SEQ_SET_ID)
 return ERROR_INV_SPS_PPS_T;
    u1_seq_parameter_set_id = u4_temp;
    COPYTHECONTEXT("SPS: seq_parameter_set_id",
                    u1_seq_parameter_set_id);

  
  
  

    ps_seq = ps_dec->pv_scratch_sps_pps;
 *ps_seq = ps_dec->ps_sps[u1_seq_parameter_set_id];

 if(NULL == ps_dec->ps_cur_sps)
        ps_dec->ps_cur_sps = ps_seq;

    ps_seq->u1_profile_idc = u1_profile_idc;
    ps_seq->u1_level_idc = u1_level_idc;
    ps_seq->u1_seq_parameter_set_id = u1_seq_parameter_set_id;

  
  
  
    ps_seq->i4_chroma_format_idc = 1;
    ps_seq->i4_bit_depth_luma_minus8 = 0;
    ps_seq->i4_bit_depth_chroma_minus8 = 0;
    ps_seq->i4_qpprime_y_zero_transform_bypass_flag = 0;
    ps_seq->i4_seq_scaling_matrix_present_flag = 0;
 if(u1_profile_idc == HIGH_PROFILE_IDC)
 {

  
        ps_seq->i4_chroma_format_idc = ih264d_uev(pu4_bitstrm_ofst,
                                                  pu4_bitstrm_buf);

  
 if(ps_seq->i4_chroma_format_idc != 1)
 {
 return ERROR_INV_SPS_PPS_T;
 }

  
        ps_seq->i4_bit_depth_luma_minus8 = ih264d_uev(pu4_bitstrm_ofst,
                                                      pu4_bitstrm_buf);

 if(ps_seq->i4_bit_depth_luma_minus8 != 0)
 {
 return ERROR_INV_SPS_PPS_T;
 }

  
        ps_seq->i4_bit_depth_chroma_minus8 = ih264d_uev(pu4_bitstrm_ofst,
                                                        pu4_bitstrm_buf);

 if(ps_seq->i4_bit_depth_chroma_minus8 != 0)
 {
 return ERROR_INV_SPS_PPS_T;
 }

  
        ps_seq->i4_qpprime_y_zero_transform_bypass_flag =
 (WORD32)ih264d_get_bit_h264(ps_bitstrm);

 if(ps_seq->i4_qpprime_y_zero_transform_bypass_flag != 0)
 {
 return ERROR_INV_SPS_PPS_T;
 }

  
        ps_seq->i4_seq_scaling_matrix_present_flag =
 (WORD32)ih264d_get_bit_h264(ps_bitstrm);

 if(ps_seq->i4_seq_scaling_matrix_present_flag)
 {
 for(i4_i = 0; i4_i < 8; i4_i++)
 {
                ps_seq->u1_seq_scaling_list_present_flag[i4_i] =
                                ih264d_get_bit_h264(ps_bitstrm);

  
  
                ps_seq->u1_use_default_scaling_matrix_flag[i4_i] = 0;

 if(ps_seq->u1_seq_scaling_list_present_flag[i4_i])
 {
 if(i4_i < 6)
 {
                        ih264d_scaling_list(
                                        ps_seq->i2_scalinglist4x4[i4_i],
 16,
 &ps_seq->u1_use_default_scaling_matrix_flag[i4_i],
                                        ps_bitstrm);
 }
 else
 {
                        ih264d_scaling_list(
                                        ps_seq->i2_scalinglist8x8[i4_i - 6],
 64,
 &ps_seq->u1_use_default_scaling_matrix_flag[i4_i],
                                        ps_bitstrm);
 }
 }
 }
 }
 }
  
  
  
    u4_temp = 4 + ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp > MAX_BITS_IN_FRAME_NUM)
 {
 return ERROR_INV_SPS_PPS_T;
 }
    ps_seq->u1_bits_in_frm_num = u4_temp;
    COPYTHECONTEXT("SPS: log2_max_frame_num_minus4",
 (ps_seq->u1_bits_in_frm_num - 4));

    i2_max_frm_num = (1 << (ps_seq->u1_bits_in_frm_num));
    ps_seq->u2_u4_max_pic_num_minus1 = i2_max_frm_num - 1;
  
  
  
    u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);

 if(u4_temp > MAX_PIC_ORDER_CNT_TYPE)
 {
 return ERROR_INV_POC_TYPE_T;
 }
    ps_seq->u1_pic_order_cnt_type = u4_temp;
    COPYTHECONTEXT("SPS: pic_order_cnt_type",ps_seq->u1_pic_order_cnt_type);

    ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle = 1;
 if(ps_seq->u1_pic_order_cnt_type == 0)
 {
        u4_temp = 4 + ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp > MAX_BITS_IN_POC_LSB)
 {
 return ERROR_INV_SPS_PPS_T;
 }
        ps_seq->u1_log2_max_pic_order_cnt_lsb_minus = u4_temp;
        ps_seq->i4_max_pic_order_cntLsb = (1 << u4_temp);
        COPYTHECONTEXT("SPS: log2_max_pic_order_cnt_lsb_minus4",(u4_temp - 4));
 }
 else if(ps_seq->u1_pic_order_cnt_type == 1)
 {
        ps_seq->u1_delta_pic_order_always_zero_flag = ih264d_get_bit_h264(
                        ps_bitstrm);
        COPYTHECONTEXT("SPS: delta_pic_order_always_zero_flag",
                        ps_seq->u1_delta_pic_order_always_zero_flag);

        ps_seq->i4_ofst_for_non_ref_pic = ih264d_sev(pu4_bitstrm_ofst,
                                                     pu4_bitstrm_buf);
        COPYTHECONTEXT("SPS: offset_for_non_ref_pic",
                        ps_seq->i4_ofst_for_non_ref_pic);

        ps_seq->i4_ofst_for_top_to_bottom_field = ih264d_sev(
                        pu4_bitstrm_ofst, pu4_bitstrm_buf);
        COPYTHECONTEXT("SPS: offset_for_top_to_bottom_field",
                        ps_seq->i4_ofst_for_top_to_bottom_field);

        u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp > 255)
 return ERROR_INV_SPS_PPS_T;
        ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle = u4_temp;
        COPYTHECONTEXT("SPS: num_ref_frames_in_pic_order_cnt_cycle",
                        ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle);

 for(i = 0; i < ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle; i++)
 {
            ps_seq->i4_ofst_for_ref_frame[i] = ih264d_sev(
                            pu4_bitstrm_ofst, pu4_bitstrm_buf);
            COPYTHECONTEXT("SPS: offset_for_ref_frame",
                            ps_seq->i4_ofst_for_ref_frame[i]);
 }
 }

    u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);

 if((u4_temp > H264_MAX_REF_PICS))
 {
 return ERROR_NUM_REF;
 }
    ps_seq->u1_num_ref_frames = u4_temp;
    COPYTHECONTEXT("SPS: num_ref_frames",ps_seq->u1_num_ref_frames);

    ps_seq->u1_gaps_in_frame_num_value_allowed_flag = ih264d_get_bit_h264(
                    ps_bitstrm);
    COPYTHECONTEXT("SPS: gaps_in_frame_num_value_allowed_flag",
                    ps_seq->u1_gaps_in_frame_num_value_allowed_flag);

  
  
  
    ps_seq->u2_frm_wd_in_mbs = 1
 + ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
    COPYTHECONTEXT("SPS: pic_width_in_mbs_minus1",
                    ps_seq->u2_frm_wd_in_mbs - 1);
    u2_pic_wd = (ps_seq->u2_frm_wd_in_mbs << 4);

    pic_height_in_map_units_minus1 = ih264d_uev(pu4_bitstrm_ofst,
                                                pu4_bitstrm_buf);
    ps_seq->u2_frm_ht_in_mbs = 1 + pic_height_in_map_units_minus1;

    u2_pic_ht = (ps_seq->u2_frm_ht_in_mbs << 4);

  
  
  
    ps_seq->u2_max_mb_addr = (ps_seq->u2_frm_wd_in_mbs
 * ps_seq->u2_frm_ht_in_mbs) - 1;

    ps_seq->u2_total_num_of_mbs = ps_seq->u2_max_mb_addr + 1;

    ps_seq->u1_level_idc = ih264d_correct_level_idc(
                    u1_level_idc, ps_seq->u2_total_num_of_mbs);

    u1_frm = ih264d_get_bit_h264(ps_bitstrm);
    ps_seq->u1_frame_mbs_only_flag = u1_frm;

    COPYTHECONTEXT("SPS: frame_mbs_only_flag", u1_frm);

 if(!u1_frm)
 {
        u2_pic_ht <<= 1;
        ps_seq->u1_mb_aff_flag = ih264d_get_bit_h264(ps_bitstrm);
        COPYTHECONTEXT("SPS: mb_adaptive_frame_field_flag",
                        ps_seq->u1_mb_aff_flag);

 }
 else
        ps_seq->u1_mb_aff_flag = 0;

    ps_seq->u1_direct_8x8_inference_flag = ih264d_get_bit_h264(ps_bitstrm);

    COPYTHECONTEXT("SPS: direct_8x8_inference_flag",
                    ps_seq->u1_direct_8x8_inference_flag);

  
    u1_frame_cropping_flag = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("SPS: frame_cropping_flag",u1_frame_cropping_flag);

 if(u1_frame_cropping_flag)
 {
        u1_frame_cropping_rect_left_ofst = ih264d_uev(pu4_bitstrm_ofst,
                                                      pu4_bitstrm_buf);
        COPYTHECONTEXT("SPS: frame_cropping_rect_left_offset",
                        u1_frame_cropping_rect_left_ofst);
        u1_frame_cropping_rect_right_ofst = ih264d_uev(pu4_bitstrm_ofst,
                                                       pu4_bitstrm_buf);
        COPYTHECONTEXT("SPS: frame_cropping_rect_right_offset",
                        u1_frame_cropping_rect_right_ofst);
        u1_frame_cropping_rect_top_ofst = ih264d_uev(pu4_bitstrm_ofst,
                                                     pu4_bitstrm_buf);
        COPYTHECONTEXT("SPS: frame_cropping_rect_top_offset",
                        u1_frame_cropping_rect_top_ofst);
        u1_frame_cropping_rect_bottom_ofst = ih264d_uev(pu4_bitstrm_ofst,
                                                        pu4_bitstrm_buf);
        COPYTHECONTEXT("SPS: frame_cropping_rect_bottom_offset",
                        u1_frame_cropping_rect_bottom_ofst);
 }
  

    ps_seq->u1_vui_parameters_present_flag = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("SPS: vui_parameters_present_flag",
                    ps_seq->u1_vui_parameters_present_flag);

    u2_frm_wd_y = u2_pic_wd + (UWORD8)(PAD_LEN_Y_H << 1);
 if(1 == ps_dec->u4_share_disp_buf)
 {
 if(ps_dec->u4_app_disp_width > u2_frm_wd_y)
            u2_frm_wd_y = ps_dec->u4_app_disp_width;
 }

    u2_frm_ht_y = u2_pic_ht + (UWORD8)(PAD_LEN_Y_V << 2);
    u2_frm_wd_uv = u2_pic_wd + (UWORD8)(PAD_LEN_UV_H << 2);
    u2_frm_wd_uv = MAX(u2_frm_wd_uv, u2_frm_wd_y);

    u2_frm_ht_uv = (u2_pic_ht >> 1) + (UWORD8)(PAD_LEN_UV_V << 2);
    u2_frm_ht_uv = MAX(u2_frm_ht_uv, (u2_frm_ht_y >> 1));


  
  
 {
        UWORD16 u2_rgt_ofst = 0;
        UWORD16 u2_lft_ofst = 0;
        UWORD16 u2_top_ofst = 0;
        UWORD16 u2_btm_ofst = 0;
        UWORD8 u1_frm_mbs_flag;
        UWORD8 u1_vert_mult_factor;
        WORD32 i4_cropped_ht, i4_cropped_wd;

 if(u1_frame_cropping_flag)
 {
  
            u2_rgt_ofst = u1_frame_cropping_rect_right_ofst << 1;
            u2_lft_ofst = u1_frame_cropping_rect_left_ofst << 1;

  
            u1_frm_mbs_flag = (1 == ps_seq->u1_frame_mbs_only_flag);

  
            u1_vert_mult_factor = (2 - u1_frm_mbs_flag);

  
            u2_btm_ofst = (u1_frame_cropping_rect_bottom_ofst
 << u1_vert_mult_factor);
            u2_top_ofst = (u1_frame_cropping_rect_top_ofst
 << u1_vert_mult_factor);
 }

  
  
        u2_crop_offset_y = (u2_frm_wd_y * u2_top_ofst) + (u2_lft_ofst);
        u2_crop_offset_uv = (u2_frm_wd_uv * (u2_top_ofst >> 1))
 + (u2_lft_ofst >> 1) * YUV420SP_FACTOR;
  
  
        i4_cropped_ht = u2_pic_ht - (u2_btm_ofst + u2_top_ofst);
        i4_cropped_wd = u2_pic_wd - (u2_rgt_ofst + u2_lft_ofst);

 if((i4_cropped_ht < MB_SIZE) || (i4_cropped_wd < MB_SIZE))
 {
 return ERROR_INV_SPS_PPS_T;
 }

 if((3 == ps_dec->i4_header_decoded) && (ps_dec->u2_pic_wd != u2_pic_wd))
 {
            ps_dec->u1_res_changed = 1;
 return IVD_RES_CHANGED;
 }
 if((3 == ps_dec->i4_header_decoded) && (ps_dec->u2_pic_ht != u2_pic_ht))
 {
            ps_dec->u1_res_changed = 1;
 return IVD_RES_CHANGED;
 }

  
 if((u2_pic_wd > H264_MAX_FRAME_WIDTH) || (u2_pic_ht > H264_MAX_FRAME_HEIGHT))
 {
 return IVD_STREAM_WIDTH_HEIGHT_NOT_SUPPORTED;
 }

        ps_dec->u2_disp_height = i4_cropped_ht;

        ps_dec->u2_disp_width = i4_cropped_wd;

 }

 if(1 == ps_seq->u1_vui_parameters_present_flag)
 {
        ret = ih264d_parse_vui_parametres(&ps_seq->s_vui, ps_bitstrm);
 if(ret != OK)
 return ret;
 }

    ps_dec->u2_pic_wd = u2_pic_wd;
    ps_dec->u2_pic_ht = u2_pic_ht;

  
    ps_dec->u2_frm_wd_y = u2_frm_wd_y;
    ps_dec->u2_frm_ht_y = u2_frm_ht_y;

    ps_dec->u2_frm_wd_uv = u2_frm_wd_uv;
    ps_dec->u2_frm_ht_uv = u2_frm_ht_uv;
    ps_dec->s_pad_mgr.u1_pad_len_y_v = (UWORD8)(PAD_LEN_Y_V << (1 - u1_frm));
    ps_dec->s_pad_mgr.u1_pad_len_cr_v = (UWORD8)(PAD_LEN_UV_V << (1 - u1_frm));

    ps_dec->u2_frm_wd_in_mbs = ps_seq->u2_frm_wd_in_mbs;
    ps_dec->u2_frm_ht_in_mbs = ps_seq->u2_frm_ht_in_mbs;

    ps_dec->u2_crop_offset_y = u2_crop_offset_y;
    ps_dec->u2_crop_offset_uv = u2_crop_offset_uv;

  
 if(ps_bitstrm->u4_ofst > ps_bitstrm->u4_max_ofst)
 {
 return ERROR_INV_SPS_PPS_T;
 }
    ps_seq->u1_is_valid = TRUE;
    ps_dec->ps_sps[u1_seq_parameter_set_id] = *ps_seq;

 return OK;
}
