WORD32 ih264d_parse_pps(dec_struct_t * ps_dec, dec_bit_stream_t * ps_bitstrm)
{
    UWORD8 uc_temp;
 dec_seq_params_t * ps_sps = NULL;
 dec_pic_params_t * ps_pps = NULL;
    UWORD32 *pu4_bitstrm_buf = ps_dec->ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstrm_ofst = &ps_dec->ps_bitstrm->u4_ofst;

  
    UWORD32 u4_temp;
    WORD32 i_temp;

  
    UWORD8 u1_more_data_flag;
    WORD32 i4_i;

  
  
  
    u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp & MASK_ERR_PIC_SET_ID)
 return ERROR_INV_SPS_PPS_T;
    ps_pps = ps_dec->pv_scratch_sps_pps;
 *ps_pps = ps_dec->ps_pps[u4_temp];
    ps_pps->u1_pic_parameter_set_id = (WORD8)u4_temp;
    COPYTHECONTEXT("PPS: pic_parameter_set_id",ps_pps->u1_pic_parameter_set_id);

  
  
  
    ps_pps->i4_transform_8x8_mode_flag = 0;
    ps_pps->i4_pic_scaling_matrix_present_flag = 0;

  
  
  
    u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp & MASK_ERR_SEQ_SET_ID)
 return ERROR_INV_SPS_PPS_T;
    COPYTHECONTEXT("PPS: seq_parameter_set_id",u4_temp);
    ps_sps = &ps_dec->ps_sps[u4_temp];
    ps_pps->ps_sps = ps_sps;

  
  
  
    ps_pps->u1_entropy_coding_mode = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("PPS: entropy_coding_mode_flag",ps_pps->u1_entropy_coding_mode);

    ps_pps->u1_pic_order_present_flag = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("PPS: pic_order_present_flag",ps_pps->u1_pic_order_present_flag);

  
  
  
    u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf) + 1;
 if(u4_temp != 1)
 {
        UWORD32 i4_error_code;
        i4_error_code = ERROR_FEATURE_UNAVAIL;
 return i4_error_code;
 }
    ps_pps->u1_num_slice_groups = u4_temp;
    COPYTHECONTEXT("PPS: num_slice_groups_minus1",ps_pps->u1_num_slice_groups -1);

  
  
  
    u4_temp = 1 + ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp > H264_MAX_REF_IDX)
 return ERROR_REF_IDX;
    ps_pps->u1_num_ref_idx_lx_active[0] = u4_temp;
    COPYTHECONTEXT("PPS: num_ref_idx_l0_active_minus1",
                    ps_pps->u1_num_ref_idx_lx_active[0] - 1);

    u4_temp = 1 + ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if(u4_temp > H264_MAX_REF_IDX)
 return ERROR_REF_IDX;
    ps_pps->u1_num_ref_idx_lx_active[1] = u4_temp;
    COPYTHECONTEXT("PPS: num_ref_idx_l1_active_minus1",
                    ps_pps->u1_num_ref_idx_lx_active[1] - 1);

    ps_pps->u1_wted_pred_flag = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("PPS: weighted prediction u4_flag",ps_pps->u1_wted_pred_flag);
    uc_temp = ih264d_get_bits_h264(ps_bitstrm, 2);
    COPYTHECONTEXT("PPS: weighted_bipred_idc",uc_temp);
    ps_pps->u1_wted_bipred_idc = uc_temp;

 if(ps_pps->u1_wted_bipred_idc > MAX_WEIGHT_BIPRED_IDC)
 return ERROR_INV_SPS_PPS_T;

    i_temp = 26 + ih264d_sev(pu4_bitstrm_ofst, pu4_bitstrm_buf);

 if((i_temp < 0) || (i_temp > 51))
 return ERROR_INV_RANGE_QP_T;

    ps_pps->u1_pic_init_qp = i_temp;
    COPYTHECONTEXT("PPS: pic_init_qp_minus26",ps_pps->u1_pic_init_qp - 26);

    i_temp = 26 + ih264d_sev(pu4_bitstrm_ofst, pu4_bitstrm_buf);

 if((i_temp < 0) || (i_temp > 51))
 return ERROR_INV_RANGE_QP_T;

    ps_pps->u1_pic_init_qs = i_temp;
    COPYTHECONTEXT("PPS: pic_init_qs_minus26",ps_pps->u1_pic_init_qs - 26);

    i_temp = ih264d_sev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 if((i_temp < -12) || (i_temp > 12))
 return ERROR_INV_RANGE_QP_T;
    ps_pps->i1_chroma_qp_index_offset = i_temp;
    COPYTHECONTEXT("PPS: chroma_qp_index_offset",ps_pps->i1_chroma_qp_index_offset);

  
  
  
  
    ps_pps->i1_second_chroma_qp_index_offset =
                    ps_pps->i1_chroma_qp_index_offset;

    ps_pps->u1_deblocking_filter_parameters_present_flag = ih264d_get_bit_h264(
                    ps_bitstrm);
    COPYTHECONTEXT("PPS: deblocking_filter_control_present_flag",
                    ps_pps->u1_deblocking_filter_parameters_present_flag);
    ps_pps->u1_constrained_intra_pred_flag = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("PPS: constrained_intra_pred_flag",
                    ps_pps->u1_constrained_intra_pred_flag);
    ps_pps->u1_redundant_pic_cnt_present_flag = ih264d_get_bit_h264(ps_bitstrm);
    COPYTHECONTEXT("PPS: redundant_pic_cnt_present_flag",
                    ps_pps->u1_redundant_pic_cnt_present_flag);

  
    u1_more_data_flag = MORE_RBSP_DATA(ps_bitstrm);
 if(u1_more_data_flag && (ps_pps->ps_sps->u1_profile_idc == HIGH_PROFILE_IDC))
 {
  
        ps_pps->i4_transform_8x8_mode_flag = (WORD32)ih264d_get_bit_h264(
                        ps_bitstrm);

  
        ps_pps->i4_pic_scaling_matrix_present_flag =
 (WORD32)ih264d_get_bit_h264(ps_bitstrm);

 if(ps_pps->i4_pic_scaling_matrix_present_flag)
 {
  
 for(i4_i = 0; i4_i < (6 + (ps_pps->i4_transform_8x8_mode_flag << 1)); i4_i++)
 {
                ps_pps->u1_pic_scaling_list_present_flag[i4_i] =
                                ih264d_get_bit_h264(ps_bitstrm);

 if(ps_pps->u1_pic_scaling_list_present_flag[i4_i])
 {
 if(i4_i < 6)
 {
                        ih264d_scaling_list(
                                        ps_pps->i2_pic_scalinglist4x4[i4_i],
 16,
 &ps_pps->u1_pic_use_default_scaling_matrix_flag[i4_i],
                                        ps_bitstrm);
 }
 else
 {
                        ih264d_scaling_list(
                                        ps_pps->i2_pic_scalinglist8x8[i4_i - 6],
 64,
 &ps_pps->u1_pic_use_default_scaling_matrix_flag[i4_i],
                                        ps_bitstrm);
 }
 }
 }
 }

  
        ps_pps->i1_second_chroma_qp_index_offset = ih264d_sev(
                        pu4_bitstrm_ofst, pu4_bitstrm_buf);

 if((ps_pps->i1_second_chroma_qp_index_offset + 12) > 24)
 return ERROR_INV_RANGE_QP_T;
 }

  
 if(ps_bitstrm->u4_ofst > ps_bitstrm->u4_max_ofst + 8)
 {
 return ERROR_INV_SPS_PPS_T;
 }
    ps_pps->u1_is_valid = TRUE;
    ps_dec->ps_pps[ps_pps->u1_pic_parameter_set_id] = *ps_pps;
 return OK;
}
