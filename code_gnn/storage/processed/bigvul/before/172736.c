WORD32 ih264d_parse_pmb_cavlc(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                              UWORD8 u1_mb_num,
                              UWORD8 u1_num_mbsNby2)
{
    UWORD32 u1_num_mb_part;
    UWORD32 uc_sub_mb;
 dec_bit_stream_t * const ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 * const pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;

 parse_pmbarams_t * ps_parse_mb_data = ps_dec->ps_parse_mb_data
 + u1_num_mbsNby2;
    WORD8 * pi1_ref_idx = ps_parse_mb_data->i1_ref_idx[0];
 const UWORD8 u1_mbaff = ps_dec->ps_cur_slice->u1_mbaff_frame_flag;
 const UWORD8 * pu1_num_mb_part = (const UWORD8 *)gau1_ih264d_num_mb_part;
    UWORD8 * pu1_col_info = ps_parse_mb_data->u1_col_info;

    UWORD32 u1_mb_type = ps_cur_mb_info->u1_mb_type;
    UWORD32 u4_sum_mb_mode_pack = 0;
    WORD32 ret;

    UWORD8 u1_no_submb_part_size_lt8x8_flag = 1;
    ps_cur_mb_info->u1_tran_form8x8 = 0;
    ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

    ps_cur_mb_info->u1_yuv_dc_block_flag = 0;

    ps_cur_mb_info->u1_mb_mc_mode = u1_mb_type;
    uc_sub_mb = ((u1_mb_type == PRED_8x8) | (u1_mb_type == PRED_8x8R0));

  
 if(uc_sub_mb)
 {
        WORD32 i;
        UWORD8 u1_colz = (PRED_8x8 << 6);

 for(i = 0; i < 4; i++)
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

 if(ui_sub_mb_mode > 3)
 {
 return ERROR_SUB_MB_TYPE;
 }
 else
 {
                u4_sum_mb_mode_pack = (u4_sum_mb_mode_pack << 8) | ui_sub_mb_mode;
  
 *pu1_col_info++ = u1_colz | (UWORD8)(ui_sub_mb_mode << 4);

                COPYTHECONTEXT("sub_mb_type", ui_sub_mb_mode);
 }

  
 if(ui_sub_mb_mode != P_L0_8x8)
 {
                u1_no_submb_part_size_lt8x8_flag = 0;
 }
 }

        u1_num_mb_part = 4;
 }
 else
 {
 *pu1_col_info++ = (u1_mb_type << 6);
 if(u1_mb_type)
 *pu1_col_info++ = (u1_mb_type << 6);
        u1_num_mb_part = pu1_num_mb_part[u1_mb_type];

 }

  
  
  

 {

        UWORD8 uc_field = ps_cur_mb_info->u1_mb_field_decodingflag;
        UWORD8 uc_num_ref_idx_l0_active_minus1 =
 (ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[0]
 << (u1_mbaff & uc_field)) - 1;

 if((uc_num_ref_idx_l0_active_minus1 > 0) & (u1_mb_type != PRED_8x8R0))
 {
 if(1 == uc_num_ref_idx_l0_active_minus1)
                ih264d_parse_pmb_ref_index_cavlc_range1(
                                u1_num_mb_part, ps_bitstrm, pi1_ref_idx,
                                uc_num_ref_idx_l0_active_minus1);
 else
 {
                ret = ih264d_parse_pmb_ref_index_cavlc(
                                u1_num_mb_part, ps_bitstrm, pi1_ref_idx,
                                uc_num_ref_idx_l0_active_minus1);
 if(ret != OK)
 return ret;
 }
 }
 else
 {
  
            UWORD8 uc_i;
 for(uc_i = 0; uc_i < u1_num_mb_part; uc_i++)
  
                pi1_ref_idx[uc_i] = 0;
 }
 }

 {
        UWORD8 u1_p_idx, uc_i;
 parse_part_params_t * ps_part = ps_dec->ps_part;
        UWORD8 u1_sub_mb_mode, u1_num_subpart, u1_mb_part_width, u1_mb_part_height;
        UWORD8 u1_sub_mb_num;
 const UWORD8 * pu1_top_left_sub_mb_indx;
 mv_pred_t * ps_mv, *ps_mv_start = ps_dec->ps_mv_cur + (u1_mb_num << 4);
  
 const UWORD8 * pu1_mb_partw = (const UWORD8 *)gau1_ih264d_mb_partw;
 const UWORD8 * pu1_mb_parth = (const UWORD8 *)gau1_ih264d_mb_parth;
 const UWORD8 * pu1_sub_mb_indx_mod =
 (const UWORD8 *)(gau1_ih264d_submb_indx_mod)
 + (uc_sub_mb * 6);
 const UWORD8 * pu1_sub_mb_partw = (const UWORD8 *)gau1_ih264d_submb_partw;
 const UWORD8 * pu1_sub_mb_parth = (const UWORD8 *)gau1_ih264d_submb_parth;
 const UWORD8 * pu1_num_sub_mb_part =
 (const UWORD8 *)gau1_ih264d_num_submb_part;

        UWORD16 u2_sub_mb_num = 0x028A;

  
  
  
  
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
                u1_sub_mb_mode = u4_sum_mb_mode_pack >> 24;
                u1_num_subpart = pu1_num_sub_mb_part[u1_sub_mb_mode];
                u1_mb_part_width = pu1_sub_mb_partw[u1_sub_mb_mode];
                u1_mb_part_height = pu1_sub_mb_parth[u1_sub_mb_mode];
                pu1_top_left_sub_mb_indx = pu1_sub_mb_indx_mod + (u1_sub_mb_mode << 1);
                u1_sub_mb_num = u2_sub_mb_num >> 12;
                u4_sum_mb_mode_pack <<= 8;
                u2_sub_mb_num <<= 4;
 }

  
 for(uc_j = 0; uc_j < u1_num_subpart; uc_j++, pu1_top_left_sub_mb_indx++)
 {
                WORD16 i2_mvx, i2_mvy;
                u1_sub_mb_num += *pu1_top_left_sub_mb_indx;
                ps_mv = ps_mv_start + u1_sub_mb_num;

  
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
                COPYTHECONTEXT("MVD", i2_mvx);
                i2_mvy = ih264d_sev(pu4_bitstrm_ofst,
                                     pu4_bitstrm_buf);
                COPYTHECONTEXT("MVD", i2_mvy);

  
                ps_part->u1_is_direct = PART_NOT_DIRECT;
                ps_part->u1_sub_mb_num = u1_sub_mb_num;
                ps_part->u1_partheight = u1_mb_part_height;
                ps_part->u1_partwidth = u1_mb_part_width;

  
                ps_mv->i2_mv[0] = i2_mvx;
                ps_mv->i2_mv[1] = i2_mvy;

  
                u1_p_idx++;
                ps_part++;
 }
 }
        ps_parse_mb_data->u1_num_part = u1_p_idx;
        ps_dec->ps_part = ps_part;
 }

 {
        UWORD32 u4_cbp;

  
        UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
        UWORD32 u4_word, u4_ldz;

  
  
  
        NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
        u4_ldz = CLZ(u4_word);
  
        u4_bitstream_offset += (u4_ldz + 1);
  
        u4_word = 0;
 if(u4_ldz)
            GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf, u4_ldz);
 *pu4_bitstrm_ofst = u4_bitstream_offset;
        u4_cbp = ((1 << u4_ldz) + u4_word - 1);

 if(u4_cbp > 47)
 return ERROR_CBP;

        u4_cbp = *((UWORD8*)gau1_ih264d_cbp_inter + u4_cbp);
        COPYTHECONTEXT("coded_block_pattern", u4_cbp);
        ps_cur_mb_info->u1_cbp = u4_cbp;

  
 if((ps_dec->s_high_profile.u1_transform8x8_present) && (u4_cbp & 0xf)
 && u1_no_submb_part_size_lt8x8_flag)
 {
            ps_cur_mb_info->u1_tran_form8x8 = ih264d_get_bit_h264(ps_bitstrm);
            COPYTHECONTEXT("transform_size_8x8_flag", ps_cur_mb_info->u1_tran_form8x8);
            ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = ps_cur_mb_info->u1_tran_form8x8;
 }

  
 if(u4_cbp)
 {
            WORD32 i_temp;

            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
            UWORD32 u4_word, u4_ldz, u4_abs_val;

  
  
  
            NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
            u4_ldz = CLZ(u4_word);

  
            u4_bitstream_offset += (u4_ldz + 1);

  
            u4_word = 0;
 if(u4_ldz)
                GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf,
                        u4_ldz);

 *pu4_bitstrm_ofst = u4_bitstream_offset;
            u4_abs_val = ((1 << u4_ldz) + u4_word) >> 1;

 if(u4_word & 0x1)
                i_temp = (-(WORD32)u4_abs_val);
 else
                i_temp = (u4_abs_val);

 if((i_temp < -26) || (i_temp > 25))
 return ERROR_INV_RANGE_QP_T;

            COPYTHECONTEXT("mb_qp_delta", i_temp);
 if(i_temp)
 {
                ret = ih264d_update_qp(ps_dec, (WORD8)i_temp);
 if(ret != OK)
 return ret;
 }

            ret = ih264d_parse_residual4x4_cavlc(ps_dec, ps_cur_mb_info, 0);
 if(ret != OK)
 return ret;
 if(EXCEED_OFFSET(ps_bitstrm))
 return ERROR_EOB_TERMINATE_T;
 }
 else
 {
            ih264d_update_nnz_for_skipmb(ps_dec, ps_cur_mb_info, CAVLC);
 }



 }

 return OK;
}
