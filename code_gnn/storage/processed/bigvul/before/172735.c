WORD32 ih264d_parse_pmb_cabac(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                              UWORD8 u1_mb_num,
                              UWORD8 u1_num_mbsNby2)
{
    UWORD32 u1_num_mb_part;
    UWORD32 uc_sub_mb;
 parse_pmbarams_t * ps_parse_mb_data = ps_dec->ps_parse_mb_data
 + u1_num_mbsNby2;
    WORD8 * pi1_ref_idx = ps_parse_mb_data->i1_ref_idx[0];
 const UWORD8 * pu1_num_mb_part = (const UWORD8 *)gau1_ih264d_num_mb_part;
 const UWORD32 u1_mb_type = ps_cur_mb_info->u1_mb_type;
    UWORD8 * pu1_col_info = ps_parse_mb_data->u1_col_info;
    UWORD32 u1_mb_mc_mode = u1_mb_type;
 ctxt_inc_mb_info_t * p_curr_ctxt = ps_dec->ps_curr_ctxt_mb_info;
 decoding_envirnoment_t * ps_cab_env = &ps_dec->s_cab_dec_env;
 dec_bit_stream_t * ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 u4_sub_mb_pack = 0;
    WORD32 ret;

    UWORD8 u1_no_submb_part_size_lt8x8_flag = 1;
    ps_cur_mb_info->u1_tran_form8x8 = 0;
    ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

    ps_cur_mb_info->u1_yuv_dc_block_flag = 0;

    p_curr_ctxt->u1_mb_type = CAB_P;
    ps_cur_mb_info->u1_mb_mc_mode = u1_mb_type;
    uc_sub_mb = ((u1_mb_type == PRED_8x8) | (u1_mb_type == PRED_8x8R0));

  
 if(uc_sub_mb)
 {

        UWORD8 u1_colz = (PRED_8x8 << 6);
        u1_mb_mc_mode = 0;

 {
            UWORD8 u1_sub_mb_mode;
            u1_sub_mb_mode = ih264d_parse_submb_type_cabac(
 0, ps_cab_env, ps_bitstrm,
                            ps_dec->p_sub_mb_type_t);
 if(u1_sub_mb_mode > 3)
 return ERROR_SUB_MB_TYPE;

            u4_sub_mb_pack = (u4_sub_mb_pack << 8) | u1_sub_mb_mode;
  
 *pu1_col_info++ = u1_colz | ((UWORD8)(u1_sub_mb_mode << 4));
            COPYTHECONTEXT("sub_mb_type", u1_sub_mb_mode);
  
 if(u1_sub_mb_mode != P_L0_8x8)
 {
                u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
 {
            UWORD8 u1_sub_mb_mode;
            u1_sub_mb_mode = ih264d_parse_submb_type_cabac(
 0, ps_cab_env, ps_bitstrm,
                            ps_dec->p_sub_mb_type_t);
 if(u1_sub_mb_mode > 3)
 return ERROR_SUB_MB_TYPE;

            u4_sub_mb_pack = (u4_sub_mb_pack << 8) | u1_sub_mb_mode;
  
 *pu1_col_info++ = u1_colz | ((UWORD8)(u1_sub_mb_mode << 4));
            COPYTHECONTEXT("sub_mb_type", u1_sub_mb_mode);
  
 if(u1_sub_mb_mode != P_L0_8x8)
 {
                u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
 {
            UWORD8 u1_sub_mb_mode;
            u1_sub_mb_mode = ih264d_parse_submb_type_cabac(
 0, ps_cab_env, ps_bitstrm,
                            ps_dec->p_sub_mb_type_t);
 if(u1_sub_mb_mode > 3)
 return ERROR_SUB_MB_TYPE;

            u4_sub_mb_pack = (u4_sub_mb_pack << 8) | u1_sub_mb_mode;
  
 *pu1_col_info++ = u1_colz | ((UWORD8)(u1_sub_mb_mode << 4));
            COPYTHECONTEXT("sub_mb_type", u1_sub_mb_mode);
  
 if(u1_sub_mb_mode != P_L0_8x8)
 {
                u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
 {
            UWORD8 u1_sub_mb_mode;
            u1_sub_mb_mode = ih264d_parse_submb_type_cabac(
 0, ps_cab_env, ps_bitstrm,
                            ps_dec->p_sub_mb_type_t);
 if(u1_sub_mb_mode > 3)
 return ERROR_SUB_MB_TYPE;

            u4_sub_mb_pack = (u4_sub_mb_pack << 8) | u1_sub_mb_mode;
  
 *pu1_col_info++ = u1_colz | ((UWORD8)(u1_sub_mb_mode << 4));
            COPYTHECONTEXT("sub_mb_type", u1_sub_mb_mode);
  
 if(u1_sub_mb_mode != P_L0_8x8)
 {
                u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
        u1_num_mb_part = 4;
 }
 else
 {
        u1_num_mb_part = pu1_num_mb_part[u1_mb_type];
  
 *pu1_col_info++ = (u1_mb_type << 6);
 if(u1_mb_type)
 *pu1_col_info++ = (u1_mb_type << 6);
 }
  
  
  
 {
        WORD8 * pi1_top_ref_idx_ctx_inc_arr = p_curr_ctxt->i1_ref_idx;
        WORD8 * pi1_left_ref_idx_ctxt_inc = ps_dec->pi1_left_ref_idx_ctxt_inc;
        UWORD8 u1_mbaff = ps_dec->ps_cur_slice->u1_mbaff_frame_flag;
        UWORD8 uc_field = ps_cur_mb_info->u1_mb_field_decodingflag;
        UWORD8 uc_num_ref_idx_l0_active_minus1 =
 (ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[0]
 << (u1_mbaff & uc_field)) - 1;

 if((uc_num_ref_idx_l0_active_minus1 > 0) & (u1_mb_type != PRED_8x8R0))
 {
  
 *((UWORD32 *)pi1_ref_idx) = 0x01010101;
            ret = ih264d_parse_ref_idx_cabac(u1_num_mb_part, 0,
                                             uc_num_ref_idx_l0_active_minus1,
                                             u1_mb_mc_mode, pi1_ref_idx,
                                             pi1_left_ref_idx_ctxt_inc,
                                             pi1_top_ref_idx_ctx_inc_arr, ps_cab_env,
                                             ps_bitstrm, ps_dec->p_ref_idx_t);
 if(ret != OK)
 return ret;
 }
 else
 {
  
            pi1_left_ref_idx_ctxt_inc[0] = 0;
            pi1_left_ref_idx_ctxt_inc[1] = 0;
            pi1_top_ref_idx_ctx_inc_arr[0] = 0;
            pi1_top_ref_idx_ctx_inc_arr[1] = 0;
 *((UWORD32 *)pi1_ref_idx) = 0;
 }
 }

 {
        UWORD8 u1_p_idx, uc_i;
 parse_part_params_t * ps_part = ps_dec->ps_part;
        UWORD8 u1_sub_mb_mode, u1_num_subpart, u1_mb_part_width, u1_mb_part_height;
        UWORD8 u1_sub_mb_num;
 const UWORD8 * pu1_top_left_sub_mb_indx;
 mv_pred_t *ps_mv_start = ps_dec->ps_mv_cur + (u1_mb_num << 4);
        UWORD16 u2_sub_mb_num_pack = 0x028A;

  
 const UWORD8 * pu1_mb_partw = (const UWORD8 *)gau1_ih264d_mb_partw;
 const UWORD8 * pu1_mb_parth = (const UWORD8 *)gau1_ih264d_mb_parth;
 const UWORD8 * pu1_sub_mb_indx_mod =
 (const UWORD8 *)(gau1_ih264d_submb_indx_mod)
 + (uc_sub_mb * 6);
 const UWORD8 * pu1_sub_mb_partw = (const UWORD8 *)gau1_ih264d_submb_partw;
 const UWORD8 * pu1_sub_mb_parth = (const UWORD8 *)gau1_ih264d_submb_parth;
 const UWORD8 * pu1_num_sub_mb_part =
 (const UWORD8 *)gau1_ih264d_num_submb_part;

  
  
  
  
        u1_sub_mb_mode = 0;
        u1_num_subpart = 1;
        u1_mb_part_width = pu1_mb_partw[u1_mb_type];
        u1_mb_part_height = pu1_mb_parth[u1_mb_type];
        pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod + (u1_mb_type << 1);
        u1_sub_mb_num = 0;

  
 for(uc_i = 0, u1_p_idx = 0; uc_i < u1_num_mb_part; uc_i++)
 {
            UWORD8 uc_j;
 if(uc_sub_mb)
 {
                u1_sub_mb_mode = u4_sub_mb_pack >> 24;
                u1_num_subpart = pu1_num_sub_mb_part[u1_sub_mb_mode];
                u1_mb_part_width = pu1_sub_mb_partw[u1_sub_mb_mode];
                u1_mb_part_height = pu1_sub_mb_parth[u1_sub_mb_mode];
                pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod + (u1_sub_mb_mode << 1);
                u1_sub_mb_num = u2_sub_mb_num_pack >> 12;
                u4_sub_mb_pack <<= 8;
                u2_sub_mb_num_pack <<= 4;
 }
  
 for(uc_j = 0; uc_j < u1_num_subpart; uc_j++, pu1_top_left_sub_mb_indx++)
 {
 mv_pred_t * ps_mv;

                u1_sub_mb_num += *pu1_top_left_sub_mb_indx;
                ps_mv = ps_mv_start + u1_sub_mb_num;

  
                ps_part->u1_is_direct = PART_NOT_DIRECT;
                ps_part->u1_sub_mb_num = u1_sub_mb_num;
                ps_part->u1_partheight = u1_mb_part_height;
                ps_part->u1_partwidth = u1_mb_part_width;

  
                u1_p_idx++;
                ps_part++;

                ih264d_get_mvd_cabac(u1_sub_mb_num, 0, u1_mb_part_width,
                                     u1_mb_part_height, 1, ps_dec, ps_mv);
 }
 }
        ps_parse_mb_data->u1_num_part = u1_p_idx;
        ps_dec->ps_part = ps_part;
 }
 {
        UWORD8 u1_cbp;

  
        u1_cbp = (WORD8)ih264d_parse_ctx_cbp_cabac(ps_dec);
        COPYTHECONTEXT("coded_block_pattern", u1_cbp);
        ps_cur_mb_info->u1_cbp = u1_cbp;
        p_curr_ctxt->u1_cbp = u1_cbp;
        p_curr_ctxt->u1_intra_chroma_pred_mode = 0;
        p_curr_ctxt->u1_yuv_dc_csbp &= 0xFE;
        ps_dec->pu1_left_yuv_dc_csbp[0] &= 0x6;

 if(u1_cbp > 47)
 return ERROR_CBP;

        ps_cur_mb_info->u1_tran_form8x8 = 0;
        ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

  
 if((ps_dec->s_high_profile.u1_transform8x8_present) && (u1_cbp & 0xf)
 && u1_no_submb_part_size_lt8x8_flag)
 {
            ps_cur_mb_info->u1_tran_form8x8 = ih264d_parse_transform8x8flag_cabac(
                            ps_dec, ps_cur_mb_info);
            COPYTHECONTEXT("transform_size_8x8_flag", ps_cur_mb_info->u1_tran_form8x8);
            p_curr_ctxt->u1_transform8x8_ctxt = ps_cur_mb_info->u1_tran_form8x8;
            ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = ps_cur_mb_info->u1_tran_form8x8;

 }
 else
 {
            p_curr_ctxt->u1_transform8x8_ctxt = 0;
 }

  
 if(u1_cbp)
 {
            WORD8 c_temp;
            ret = ih264d_parse_mb_qp_delta_cabac(ps_dec, &c_temp);
 if(ret != OK)
 return ret;
            COPYTHECONTEXT("mb_qp_delta", c_temp);
 if(c_temp != 0)
 {
                ret = ih264d_update_qp(ps_dec, c_temp);
 if(ret != OK)
 return ret;
 }
 }
 else
            ps_dec->i1_prev_mb_qp_delta = 0;



        ih264d_parse_residual4x4_cabac(ps_dec, ps_cur_mb_info, 0);
 if(EXCEED_OFFSET(ps_dec->ps_bitstrm))
 return ERROR_EOB_TERMINATE_T;
 }
 return OK;
}
