WORD32 ih264d_parse_bmb_non_direct_cavlc(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                                       UWORD8 u1_mb_num,
                                       UWORD8 u1_num_mbsNby2)
{
 dec_bit_stream_t * ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
    UWORD8 * pu1_sub_mb_pred_modes = (UWORD8 *)(gau1_ih264d_submb_pred_modes) + 4;
 const UWORD8 (*pu1_mb_pred_modes)[32] =
 (const UWORD8 (*)[32])gau1_ih264d_mb_pred_modes;
 const UWORD8 * pu1_num_mb_part = (const UWORD8 *)gau1_ih264d_num_mb_part;
 const UWORD8 * pu1_sub_mb_mc_mode = (const UWORD8 *)(gau1_ih264d_submb_mc_mode)
 + 4;

 parse_pmbarams_t * ps_parse_mb_data = ps_dec->ps_parse_mb_data
 + u1_num_mbsNby2;
    UWORD8 * pu1_col_info = ps_parse_mb_data->u1_col_info;
    WORD8 (*pi1_ref_idx)[MAX_REFIDX_INFO_PER_MB] = ps_parse_mb_data->i1_ref_idx;
    UWORD8 u1_mb_type = ps_cur_mb_info->u1_mb_type;
    UWORD8 u1_mb_mc_mode, u1_num_mb_part, u1_sub_mb = !(u1_mb_type ^ B_8x8);
    UWORD32 u4_mb_mc_mode = 0, u4_mb_pred_mode = 0;
    WORD32 ret;

 if(u1_sub_mb)
 {
        UWORD8 uc_i;
        u1_mb_mc_mode = 0;
        u1_num_mb_part = 4;
  
 for(uc_i = 0; uc_i < 4; uc_i++)
 {

            UWORD32 ui_sub_mb_mode;

            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
            UWORD32 u4_word, u4_ldz;

  
  
  
            NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
            u4_ldz = CLZ(u4_word);
  
            u4_bitstream_offset += (u4_ldz + 1);
  
            u4_word = 0;
 if(u4_ldz)
                GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf,
                        u4_ldz);
 *pu4_bitstrm_ofst = u4_bitstream_offset;
            ui_sub_mb_mode = ((1 << u4_ldz) + u4_word - 1);

 if(ui_sub_mb_mode > 12)
 return ERROR_SUB_MB_TYPE;
 else
 {
                UWORD8 u1_subMbPredMode = pu1_sub_mb_pred_modes[ui_sub_mb_mode];
                u4_mb_mc_mode = (u4_mb_mc_mode << 8)
 | pu1_sub_mb_mc_mode[ui_sub_mb_mode];
                u4_mb_pred_mode = (u4_mb_pred_mode << 8) | u1_subMbPredMode;
                pi1_ref_idx[0][uc_i] = ((u1_subMbPredMode & PRED_L0) - 1) >> 1;
                pi1_ref_idx[1][uc_i] = ((u1_subMbPredMode & PRED_L1) - 1) >> 1;
                COPYTHECONTEXT("sub_mb_type", u1_subMbPredMode);
 }
  
 *pu1_col_info++ = ((PRED_8x8) << 6)
 | ((pu1_sub_mb_mc_mode[ui_sub_mb_mode] << 4));
 if(ui_sub_mb_mode != B_DIRECT_8x8)
 {
 if(ui_sub_mb_mode > B_BI_8x8)
 {
                    ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
 else if(!ps_dec->s_high_profile.u1_direct_8x8_inference_flag)
 {
                ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }
 }
 else
 {
        UWORD8 u1_mb_pred_mode_idx = 5 + u1_mb_type;
        UWORD8 u1_mb_pred_mode_part0 = pu1_mb_pred_modes[0][u1_mb_pred_mode_idx];
        UWORD8 u1_mb_pred_mode_part1 = pu1_mb_pred_modes[1][u1_mb_pred_mode_idx];
        u1_mb_mc_mode = ps_cur_mb_info->u1_mb_mc_mode;
        u1_num_mb_part = pu1_num_mb_part[u1_mb_mc_mode];

        pi1_ref_idx[0][0] = ((u1_mb_pred_mode_part0 & PRED_L0) - 1) >> 1;
        pi1_ref_idx[1][0] = ((u1_mb_pred_mode_part0 & PRED_L1) - 1) >> 1;
        pi1_ref_idx[0][1] = ((u1_mb_pred_mode_part1 & PRED_L0) - 1) >> 1;
        pi1_ref_idx[1][1] = ((u1_mb_pred_mode_part1 & PRED_L1) - 1) >> 1;

        u4_mb_pred_mode = (u1_mb_pred_mode_part0 << 8) | u1_mb_pred_mode_part1;
        u4_mb_mc_mode = u1_mb_mc_mode | (u1_mb_mc_mode << 8);
        u4_mb_mc_mode <<= 16;
        u4_mb_pred_mode <<= 16;

  
 *pu1_col_info++ = (u1_mb_mc_mode << 6);
 if(u1_mb_mc_mode)
 *pu1_col_info++ = (u1_mb_mc_mode << 6);
 }

 {
        UWORD8 u1_mbaff = ps_dec->ps_cur_slice->u1_mbaff_frame_flag;
        UWORD8 uc_field = ps_cur_mb_info->u1_mb_field_decodingflag;
        UWORD8 *pu1_num_ref_idx_lx_active =
                        ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active;
 const UWORD8 u1_mbaff_field = (u1_mbaff & uc_field);
        UWORD8 u4_num_ref_idx_lx_active;

        u4_num_ref_idx_lx_active = (pu1_num_ref_idx_lx_active[0]
 << u1_mbaff_field) - 1;

 if(u4_num_ref_idx_lx_active)
 {
 if(1 == u4_num_ref_idx_lx_active)
                ih264d_parse_bmb_ref_index_cavlc_range1(
                                u1_num_mb_part, ps_bitstrm, pi1_ref_idx[0],
                                u4_num_ref_idx_lx_active);
 else
 {
                ret = ih264d_parse_bmb_ref_index_cavlc(u1_num_mb_part, ps_bitstrm,
                                                 pi1_ref_idx[0],
                                                 u4_num_ref_idx_lx_active);
 if(ret != OK)
 return ret;
 }
 }

        u4_num_ref_idx_lx_active = (pu1_num_ref_idx_lx_active[1]
 << u1_mbaff_field) - 1;

 if(u4_num_ref_idx_lx_active)
 {
 if(1 == u4_num_ref_idx_lx_active)
                ih264d_parse_bmb_ref_index_cavlc_range1(
                                u1_num_mb_part, ps_bitstrm, pi1_ref_idx[1],
                                u4_num_ref_idx_lx_active);
 else
 {
                ret = ih264d_parse_bmb_ref_index_cavlc(u1_num_mb_part, ps_bitstrm,
                                                 pi1_ref_idx[1],
                                                 u4_num_ref_idx_lx_active);
 if(ret != OK)
 return ret;
 }
 }
 }

  
 {
 const UWORD8 * pu1_top_left_sub_mb_indx;

 const UWORD8 * pu1_sub_mb_indx_mod =
 (const UWORD8 *)(gau1_ih264d_submb_indx_mod)
 + (u1_sub_mb * 6);
 const UWORD8 * pu1_sub_mb_partw = (const UWORD8 *)gau1_ih264d_submb_partw;
 const UWORD8 * pu1_sub_mb_parth = (const UWORD8 *)gau1_ih264d_submb_parth;
 const UWORD8 * pu1_num_sub_mb_part =
 (const UWORD8 *)gau1_ih264d_num_submb_part;
 const UWORD8 * pu1_mb_partw = (const UWORD8 *)gau1_ih264d_mb_partw;
 const UWORD8 * pu1_mb_parth = (const UWORD8 *)gau1_ih264d_mb_parth;
        UWORD8 u1_p_idx = 0, u1_num_submb_part, uc_lx;
 parse_part_params_t * ps_part;
 mv_pred_t *ps_mv_start = ps_dec->ps_mv_cur + (u1_mb_num << 4);
        UWORD8 u1_mb_part_wd, u1_mb_part_ht;

  
        ps_part = ps_dec->ps_part;
  
        u1_mb_part_wd = pu1_mb_partw[u1_mb_mc_mode];
        u1_mb_part_ht = pu1_mb_parth[u1_mb_mc_mode];
        u1_num_submb_part = 1;

  
 for(uc_lx = 0; uc_lx < 2; uc_lx++)
 {
            UWORD8 u1_sub_mb_num = 0, u1_pred_mode, uc_i;
            UWORD32 u4_mb_mc_mode_tmp = u4_mb_mc_mode;
            UWORD32 u4_mb_pred_mode_tmp = u4_mb_pred_mode;
            UWORD16 u2_sub_mb_num = 0x028A;  
            UWORD8 u1_b2 = uc_lx << 1;
            u1_pred_mode = (uc_lx) ? PRED_L1 : PRED_L0;
            pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod + (u1_mb_mc_mode << 1);

 for(uc_i = 0; uc_i < u1_num_mb_part; uc_i++)
 {
                UWORD8 u1_mb_mc_mode, uc_j;
                UWORD8 i1_pred = u4_mb_pred_mode_tmp >> 24;
                u1_mb_mc_mode = u4_mb_mc_mode_tmp >> 24;
                u4_mb_pred_mode_tmp <<= 8;
                u4_mb_mc_mode_tmp <<= 8;
  
 if(u1_sub_mb)
 {

                    u1_mb_part_wd = pu1_sub_mb_partw[u1_mb_mc_mode];
                    u1_mb_part_ht = pu1_sub_mb_parth[u1_mb_mc_mode];
                    u1_sub_mb_num = u2_sub_mb_num >> 12;
                    u1_num_submb_part = pu1_num_sub_mb_part[u1_mb_mc_mode];
                    pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod
 + (u1_mb_mc_mode << 1);
                    u2_sub_mb_num <<= 4;
 }
 for(uc_j = 0; uc_j < u1_num_submb_part;
                                uc_j++, pu1_top_left_sub_mb_indx++)
 {
 mv_pred_t * ps_mv;
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

 if(i1_pred & u1_pred_mode)
 {
                        WORD16 i2_mvx, i2_mvy;

 {
                            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
                            UWORD32 u4_word, u4_ldz, u4_abs_val;

  
  
  
                            NEXTBITS_32(u4_word, u4_bitstream_offset,
                                        pu4_bitstrm_buf);
                            u4_ldz = CLZ(u4_word);

  
                            u4_bitstream_offset += (u4_ldz + 1);

  
                            u4_word = 0;
 if(u4_ldz)
                                GETBITS(u4_word, u4_bitstream_offset,
                                        pu4_bitstrm_buf, u4_ldz);

 *pu4_bitstrm_ofst = u4_bitstream_offset;
                            u4_abs_val = ((1 << u4_ldz) + u4_word) >> 1;

 if(u4_word & 0x1)
                                i2_mvx = (-(WORD32)u4_abs_val);
 else
                                i2_mvx = (u4_abs_val);
 }

 {
                            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
                            UWORD32 u4_word, u4_ldz, u4_abs_val;

  
  
  
                            NEXTBITS_32(u4_word, u4_bitstream_offset,
                                        pu4_bitstrm_buf);
                            u4_ldz = CLZ(u4_word);

  
                            u4_bitstream_offset += (u4_ldz + 1);

  
                            u4_word = 0;
 if(u4_ldz)
                                GETBITS(u4_word, u4_bitstream_offset,
                                        pu4_bitstrm_buf, u4_ldz);

 *pu4_bitstrm_ofst = u4_bitstream_offset;
                            u4_abs_val = ((1 << u4_ldz) + u4_word) >> 1;

 if(u4_word & 0x1)
                                i2_mvy = (-(WORD32)u4_abs_val);
 else
                                i2_mvy = (u4_abs_val);
 }

  
                        ps_mv->i2_mv[u1_b2] = i2_mvx;
                        ps_mv->i2_mv[u1_b2 + 1] = i2_mvy;
 }
 }
 }
 }
  
        ps_dec->ps_part = ps_part;
        ps_parse_mb_data->u1_num_part = u1_sub_mb ? u1_p_idx : u1_num_mb_part;

 }
 return OK;
}
