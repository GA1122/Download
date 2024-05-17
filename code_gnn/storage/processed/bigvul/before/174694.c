WORD32 ih264d_parse_bmb_non_direct_cabac(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                                       UWORD8 u1_mb_num,
                                       UWORD8 u1_num_mbsNby2)
{
  
 decoding_envirnoment_t * ps_cab_env = &ps_dec->s_cab_dec_env;
 dec_bit_stream_t * ps_bitstrm = ps_dec->ps_bitstrm;
 ctxt_inc_mb_info_t *p_curr_ctxt = ps_dec->ps_curr_ctxt_mb_info;
 parse_pmbarams_t * ps_parse_mb_data = ps_dec->ps_parse_mb_data
 + u1_num_mbsNby2;

  
 const UWORD8 * pu1_sub_mb_pred_modes = (UWORD8 *)(gau1_ih264d_submb_pred_modes)
 + 4;
 const UWORD8 (*pu1_mb_pred_modes)[32] =
 (const UWORD8 (*)[32])gau1_ih264d_mb_pred_modes;
 const UWORD8 *pu1_num_mb_part = (const UWORD8 *)gau1_ih264d_num_mb_part;
 const UWORD8 *pu1_sub_mb_mc_mode = (UWORD8 *)(gau1_ih264d_submb_mc_mode) + 4;

 const UWORD8 u1_mb_type = ps_cur_mb_info->u1_mb_type;
    UWORD8 * pu1_col_info = ps_parse_mb_data->u1_col_info;
    WORD8 *pi1_ref_idx_l0 = &ps_parse_mb_data->i1_ref_idx[0][0];
    WORD8 *pi1_ref_idx_l1 = &ps_parse_mb_data->i1_ref_idx[1][0];
    UWORD8 u1_dec_ref_l0, u1_dec_ref_l1;

    UWORD8 u1_num_mb_part, u1_mb_mc_mode, u1_sub_mb, u1_mbpred_mode = 5
 + u1_mb_type;
    UWORD32 u4_mb_mc_mode = 0, u4_mb_pred_mode = 0;
    WORD32 ret;

    p_curr_ctxt->u1_mb_type = CAB_NON_BD16x16;
    u1_sub_mb = !(u1_mb_type ^ B_8x8);

 {
        UWORD8 u1_mbaff = ps_dec->ps_cur_slice->u1_mbaff_frame_flag;
        UWORD8 *pu1_num_ref_idx_lx_active =
                        ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active;
        UWORD8 uc_field = ps_cur_mb_info->u1_mb_field_decodingflag;
        UWORD8 u1_mbaff_field = (u1_mbaff & uc_field);
        u1_dec_ref_l0 = (pu1_num_ref_idx_lx_active[0] << u1_mbaff_field) - 1;
        u1_dec_ref_l1 = (pu1_num_ref_idx_lx_active[1] << u1_mbaff_field) - 1;
 }

 if(u1_sub_mb)
 {
 const UWORD8 u1_colz = ((PRED_8x8) << 6);
        UWORD8 uc_i;
        u1_mb_mc_mode = 0;
        u1_num_mb_part = 4;
  
 for(uc_i = 0; uc_i < 4; uc_i++)
 {
            UWORD8 u1_sub_mb_mode, u1_subMbPredModes;
            u1_sub_mb_mode = ih264d_parse_submb_type_cabac(
 1, ps_cab_env, ps_bitstrm,
                            ps_dec->p_sub_mb_type_t);

 if(u1_sub_mb_mode > 12)
 return ERROR_SUB_MB_TYPE;

            u1_subMbPredModes = pu1_sub_mb_pred_modes[u1_sub_mb_mode];
            u4_mb_mc_mode = (u4_mb_mc_mode << 8) | pu1_sub_mb_mc_mode[u1_sub_mb_mode];
            u4_mb_pred_mode = (u4_mb_pred_mode << 8) | u1_subMbPredModes;
 *pi1_ref_idx_l0++ =
 (u1_subMbPredModes & PRED_L0) ? u1_dec_ref_l0 : -1;
 *pi1_ref_idx_l1++ =
 (u1_subMbPredModes & PRED_L1) ? u1_dec_ref_l1 : -1;
            COPYTHECONTEXT("sub_mb_type", u1_sub_mb_mode);
  
 *pu1_col_info++ =
 (u1_colz | (pu1_sub_mb_mc_mode[u1_sub_mb_mode] << 4));
 if(u1_sub_mb_mode != B_DIRECT_8x8)
 {
 if(u1_sub_mb_mode > B_BI_8x8)
 {
                    ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
 else if(!ps_dec->s_high_profile.u1_direct_8x8_inference_flag)
 {
                ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
        pi1_ref_idx_l0 -= 4;
        pi1_ref_idx_l1 -= 4;
 }
 else
 {
        UWORD8 u1_mb_pred_mode_part0 = pu1_mb_pred_modes[0][u1_mbpred_mode];
        UWORD8 u1_mb_pred_mode_part1 = pu1_mb_pred_modes[1][u1_mbpred_mode];
        u1_mb_mc_mode = ps_cur_mb_info->u1_mb_mc_mode;
        u1_num_mb_part = pu1_num_mb_part[u1_mb_mc_mode];
  
 *pu1_col_info++ = (u1_mb_mc_mode << 6);
 if(u1_mb_mc_mode)
 *pu1_col_info++ = (u1_mb_mc_mode << 6);
        u4_mb_mc_mode = u1_mb_mc_mode | (u1_mb_mc_mode << 8);
        u4_mb_mc_mode <<= 16;
        u4_mb_pred_mode = ((u1_mb_pred_mode_part0 << 8) | u1_mb_pred_mode_part1) << 16;

 *pi1_ref_idx_l0++ = (u1_mb_pred_mode_part0 & PRED_L0) ? u1_dec_ref_l0 : -1;
 *pi1_ref_idx_l0-- = (u1_mb_pred_mode_part1 & PRED_L0) ? u1_dec_ref_l0 : -1;
 *pi1_ref_idx_l1++ = (u1_mb_pred_mode_part0 & PRED_L1) ? u1_dec_ref_l1 : -1;
 *pi1_ref_idx_l1-- = (u1_mb_pred_mode_part1 & PRED_L1) ? u1_dec_ref_l1 : -1;
 }
 {
        WORD8 *pi1_lft_cxt = ps_dec->pi1_left_ref_idx_ctxt_inc;
        WORD8 *pi1_top_cxt = p_curr_ctxt->i1_ref_idx;

        ret = ih264d_parse_ref_idx_cabac(u1_num_mb_part, 0, u1_dec_ref_l0,
                                   u1_mb_mc_mode, pi1_ref_idx_l0, pi1_lft_cxt,
                                   pi1_top_cxt, ps_cab_env, ps_bitstrm,
                                   ps_dec->p_ref_idx_t);
 if(ret != OK)
 return ret;

        ret = ih264d_parse_ref_idx_cabac(u1_num_mb_part, 2, u1_dec_ref_l1,
                                   u1_mb_mc_mode, pi1_ref_idx_l1, pi1_lft_cxt,
                                   pi1_top_cxt, ps_cab_env, ps_bitstrm,
                                   ps_dec->p_ref_idx_t);
 if(ret != OK)
 return ret;
 }
  
 {
 const UWORD8 *pu1_top_left_sub_mb_indx;
        UWORD8 uc_j, uc_lx;
        UWORD8 u1_mb_part_wd, u1_mb_part_ht;

 const UWORD8 *pu1_sub_mb_indx_mod =
 (const UWORD8 *)gau1_ih264d_submb_indx_mod
 + (u1_sub_mb * 6);
 const UWORD8 *pu1_sub_mb_partw = (const UWORD8 *)gau1_ih264d_submb_partw;
 const UWORD8 *pu1_sub_mb_parth = (const UWORD8 *)gau1_ih264d_submb_parth;
 const UWORD8 *pu1_num_sub_mb_part =
 (const UWORD8 *)gau1_ih264d_num_submb_part;
 const UWORD8 *pu1_mb_partw = (const UWORD8 *)gau1_ih264d_mb_partw;
 const UWORD8 *pu1_mb_parth = (const UWORD8 *)gau1_ih264d_mb_parth;

        UWORD8 u1_p_idx = 0;
        UWORD8 u1_num_submb_part;
 parse_part_params_t *ps_part;
  
 mv_pred_t *ps_mv_start = ps_dec->ps_mv_cur + (u1_mb_num << 4);
        ps_part = ps_dec->ps_part;

  
        u1_mb_part_wd = pu1_mb_partw[u1_mb_mc_mode];
        u1_mb_part_ht = pu1_mb_parth[u1_mb_mc_mode];
        u1_num_submb_part = 1;

  
 for(uc_lx = 0; uc_lx < 2; uc_lx++)
 {
            UWORD8 u1_sub_mb_num = 0;
            UWORD32 u4_mb_pred_mode_tmp = u4_mb_pred_mode;
            UWORD32 u4_mb_mc_mode_tmp = u4_mb_mc_mode;
            UWORD8 u1_mb_mc_mode_1, u1_pred_mode, uc_i;
            UWORD16 u2_sub_mb_num = 0x028A;
            UWORD8 u1_b2 = uc_lx << 1;
            u1_pred_mode = (uc_lx) ? PRED_L1 : PRED_L0;
  
            pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod + (u1_mb_mc_mode << 1);
 for(uc_i = 0; uc_i < u1_num_mb_part; uc_i++)
 {

                WORD8 i1_pred = (UWORD8)(u4_mb_pred_mode_tmp >> 24);
                u1_mb_mc_mode_1 = (UWORD8)(u4_mb_mc_mode_tmp >> 24);
                u4_mb_pred_mode_tmp <<= 8;
                u4_mb_mc_mode_tmp <<= 8;

  
 if(u1_sub_mb)
 {
                    u1_mb_part_wd = pu1_sub_mb_partw[u1_mb_mc_mode_1];
                    u1_mb_part_ht = pu1_sub_mb_parth[u1_mb_mc_mode_1];
                    u1_sub_mb_num = u2_sub_mb_num >> 12;
                    pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod + (u1_mb_mc_mode_1 << 1);
                    u1_num_submb_part = pu1_num_sub_mb_part[u1_mb_mc_mode_1];
                    u2_sub_mb_num = u2_sub_mb_num << 4;
 }

 for(uc_j = 0; uc_j < u1_num_submb_part;
                                uc_j++, pu1_top_left_sub_mb_indx++)
 {
 mv_pred_t *ps_mv;
                    u1_sub_mb_num = u1_sub_mb_num + *pu1_top_left_sub_mb_indx;
                    ps_mv = ps_mv_start + u1_sub_mb_num;

  
 if(uc_lx)
 {
                        ps_part->u1_is_direct = (!i1_pred);
                        ps_part->u1_pred_mode = i1_pred;
                        ps_part->u1_sub_mb_num = u1_sub_mb_num;
                        ps_part->u1_partheight = u1_mb_part_ht;
                        ps_part->u1_partwidth = u1_mb_part_wd;

  
                        u1_p_idx++;
                        ps_part++;
 }

                    ih264d_get_mvd_cabac(u1_sub_mb_num, u1_b2, u1_mb_part_wd,
                                         u1_mb_part_ht,
 (UWORD8)(i1_pred & u1_pred_mode), ps_dec,
                                         ps_mv);
 }
 }
 }
  

        ps_dec->ps_part = ps_part;
        ps_parse_mb_data->u1_num_part = u1_sub_mb ? u1_p_idx : u1_num_mb_part;

 }

 return OK;
}
