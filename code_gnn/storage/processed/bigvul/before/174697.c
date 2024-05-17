WORD32 ih264d_parse_imb_cavlc(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                              UWORD8 u1_mb_num,
                              UWORD8 u1_mb_type)
{
    WORD32 i4_delta_qp;
    UWORD32 u4_temp;
    UWORD32 ui_is_top_mb_available;
    UWORD32 ui_is_left_mb_available;
    UWORD32 u4_cbp;
    UWORD32 u4_offset;
    UWORD32 *pu4_bitstrm_buf;
    WORD32 ret;

 dec_bit_stream_t * const ps_bitstrm = ps_dec->ps_bitstrm;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
    UNUSED(u1_mb_num);
    ps_cur_mb_info->u1_tran_form8x8 = 0;
    ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

    ps_cur_mb_info->u1_yuv_dc_block_flag = 0;

    u4_temp = ps_dec->u1_mb_ngbr_availablity;
    ui_is_top_mb_available = BOOLEAN(u4_temp & TOP_MB_AVAILABLE_MASK);
    ui_is_left_mb_available = BOOLEAN(u4_temp & LEFT_MB_AVAILABLE_MASK);

    pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;

 if(u1_mb_type == I_4x4_MB)
 {
        ps_cur_mb_info->ps_curmb->u1_mb_type = I_4x4_MB;
        u4_offset = 0;

  
  
  
 if(ps_dec->s_high_profile.u1_transform8x8_present)
 {
            ps_cur_mb_info->u1_tran_form8x8 = ih264d_get_bit_h264(ps_bitstrm);
            COPYTHECONTEXT("transform_size_8x8_flag", ps_cur_mb_info->u1_tran_form8x8);
            ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = ps_cur_mb_info->u1_tran_form8x8;
 }

  
  
  
 if (!ps_cur_mb_info->u1_tran_form8x8)
 {
            UWORD8 *pu1_temp;
            ih264d_read_intra_pred_modes(ps_dec,
 ((UWORD8 *)ps_dec->pv_parse_tu_coeff_data),
 ((UWORD8 *)ps_dec->pv_parse_tu_coeff_data+16),
                                          ps_cur_mb_info->u1_tran_form8x8);
            pu1_temp = (UWORD8 *)ps_dec->pv_parse_tu_coeff_data;
            pu1_temp += 32;
            ps_dec->pv_parse_tu_coeff_data = (void *)pu1_temp;
 }
 else
 {
            UWORD8 *pu1_temp;
            ih264d_read_intra_pred_modes(ps_dec,
 ((UWORD8 *)ps_dec->pv_parse_tu_coeff_data),
 ((UWORD8 *)ps_dec->pv_parse_tu_coeff_data+4),
                                          ps_cur_mb_info->u1_tran_form8x8);
            pu1_temp = (UWORD8 *)ps_dec->pv_parse_tu_coeff_data;
            pu1_temp += 8;
            ps_dec->pv_parse_tu_coeff_data = (void *)pu1_temp;
 }
  
  
  
 {
            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
            UWORD32 u4_word, u4_ldz, u4_temp;

  
  
  
            NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
            u4_ldz = CLZ(u4_word);
  
            u4_bitstream_offset += (u4_ldz + 1);
  
            u4_word = 0;
 if(u4_ldz)
 {
                GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf,
                        u4_ldz);
 }
 *pu4_bitstrm_ofst = u4_bitstream_offset;
            u4_temp = ((1 << u4_ldz) + u4_word - 1);
 if(u4_temp > 3)
 {
 return ERROR_CHROMA_PRED_MODE;
 }
            ps_cur_mb_info->u1_chroma_pred_mode = u4_temp;
            COPYTHECONTEXT("intra_chroma_pred_mode", ps_cur_mb_info->u1_chroma_pred_mode);
 }
  
  
  
 {
            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
            UWORD32 u4_word, u4_ldz;

  
  
  
            NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
            u4_ldz = CLZ(u4_word);
  
            u4_bitstream_offset += (u4_ldz + 1);
  
            u4_word = 0;
 if(u4_ldz)
 {
                GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf,
                        u4_ldz);
 }
 *pu4_bitstrm_ofst = u4_bitstream_offset;
            u4_cbp = ((1 << u4_ldz) + u4_word - 1);
 }
 if(u4_cbp > 47)
 {
 return ERROR_CBP;
 }

        u4_cbp = gau1_ih264d_cbp_table[u4_cbp][0];
        COPYTHECONTEXT("coded_block_pattern", u1_cbp);
        ps_cur_mb_info->u1_cbp = u4_cbp;

  
  
  
 if(ps_cur_mb_info->u1_cbp)
 {
            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
            UWORD32 u4_word, u4_ldz, u4_abs_val;

  
  
  
            NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
            u4_ldz = CLZ(u4_word);

  
            u4_bitstream_offset += (u4_ldz + 1);

  
            u4_word = 0;
 if(u4_ldz)
 {
                GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf,
                        u4_ldz);
 }

 *pu4_bitstrm_ofst = u4_bitstream_offset;
            u4_abs_val = ((1 << u4_ldz) + u4_word) >> 1;

 if(u4_word & 0x1)
 {
                i4_delta_qp = (-(WORD32)u4_abs_val);
 }
 else
 {
                i4_delta_qp = (u4_abs_val);
 }

 if((i4_delta_qp < -26) || (i4_delta_qp > 25))
 {
 return ERROR_INV_RANGE_QP_T;
 }

            COPYTHECONTEXT("mb_qp_delta", i1_delta_qp);
 if(i4_delta_qp != 0)
 {
                ret = ih264d_update_qp(ps_dec, (WORD8)i4_delta_qp);
 if(ret != OK)
 return ret;
 }
 }

 }
 else
 {
        u4_offset = 1;
        ps_cur_mb_info->ps_curmb->u1_mb_type = I_16x16_MB;
  
  
  
 {
            UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
            UWORD32 u4_word, u4_ldz;

  
  
  
            NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
            u4_ldz = CLZ(u4_word);
  
            u4_bitstream_offset += (u4_ldz + 1);
  
            u4_word = 0;
 if(u4_ldz)
 {
                GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf,
                        u4_ldz);
 }
 *pu4_bitstrm_ofst = u4_bitstream_offset;
            u4_temp = ((1 << u4_ldz) + u4_word - 1);


 if(u4_temp > 3)
 {
 return ERROR_CHROMA_PRED_MODE;
 }
            ps_cur_mb_info->u1_chroma_pred_mode = u4_temp;
            COPYTHECONTEXT("intra_chroma_pred_mode", ps_cur_mb_info->u1_chroma_pred_mode);
 }
  
  
  
        u4_cbp = gau1_ih264d_cbp_tab[(u1_mb_type - 1) >> 2];
        ps_cur_mb_info->u1_cbp = u4_cbp;

  
  
  
 {
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
                i4_delta_qp = (-(WORD32)u4_abs_val);
 else
                i4_delta_qp = (u4_abs_val);

 if((i4_delta_qp < -26) || (i4_delta_qp > 25))
 return ERROR_INV_RANGE_QP_T;

 }
        COPYTHECONTEXT("Delta quant", i1_delta_qp);

 if(i4_delta_qp != 0)
 {
            ret = ih264d_update_qp(ps_dec, (WORD8)i4_delta_qp);
 if(ret != OK)
 return ret;
 }

 {
            WORD16 i_scaleFactor;
            UWORD32 ui_N = 0;
            WORD16 *pi2_scale_matrix_ptr;
  
  
  
  
 if(ps_dec->s_high_profile.u1_scaling_present)
 {
                pi2_scale_matrix_ptr =
                                ps_dec->s_high_profile.i2_scalinglist4x4[0];
 }
 else
 {
                i_scaleFactor = 16;
                pi2_scale_matrix_ptr = &i_scaleFactor;
 }

  
  
  
  
  
  
 if(ui_is_left_mb_available)
 {

 if(ui_is_top_mb_available)
 {
                    ui_N = ((ps_cur_mb_info->ps_top_mb->pu1_nnz_y[0]
 + ps_dec->pu1_left_nnz_y[0] + 1) >> 1);
 }
 else
 {
                    ui_N = ps_dec->pu1_left_nnz_y[0];
 }
 }
 else if(ui_is_top_mb_available)
 {
                ui_N = ps_cur_mb_info->ps_top_mb->pu1_nnz_y[0];
 }

 {
                WORD16 pi2_dc_coef[16];
                WORD32 pi4_tmp[16];
 tu_sblk4x4_coeff_data_t *ps_tu_4x4 =
 (tu_sblk4x4_coeff_data_t *)ps_dec->pv_parse_tu_coeff_data;
                WORD16 *pi2_coeff_block =
 (WORD16 *)ps_dec->pv_parse_tu_coeff_data;
                UWORD32 u4_num_coeff;
                ps_tu_4x4->u2_sig_coeff_map = 0;

                ret = ps_dec->pf_cavlc_parse4x4coeff[(ui_N > 7)](pi2_dc_coef, 0, ui_N,
                                                                 ps_dec, &u4_num_coeff);
 if(ret != OK)
 return ret;

 if(EXCEED_OFFSET(ps_bitstrm))
 return ERROR_EOB_TERMINATE_T;
 if(ps_tu_4x4->u2_sig_coeff_map)
 {
                    memset(pi2_dc_coef,0,sizeof(pi2_dc_coef));
                    ih264d_unpack_coeff4x4_dc_4x4blk(ps_tu_4x4,
                                                     pi2_dc_coef,
                                                     ps_dec->pu1_inv_scan);

                    PROFILE_DISABLE_IQ_IT_RECON()
                    ps_dec->pf_ihadamard_scaling_4x4(pi2_dc_coef,
                                                     pi2_coeff_block,
                                                     ps_dec->pu2_quant_scale_y,
 (UWORD16 *)pi2_scale_matrix_ptr,
                                                     ps_dec->u1_qp_y_div6,
                                                     pi4_tmp);
                    pi2_coeff_block += 16;
                    ps_dec->pv_parse_tu_coeff_data = (void *)pi2_coeff_block;
                    SET_BIT(ps_cur_mb_info->u1_yuv_dc_block_flag,0);
 }

 }
 }
 }


 if(u4_cbp)
 {

        ret = ih264d_parse_residual4x4_cavlc(ps_dec, ps_cur_mb_info,
 (UWORD8)u4_offset);
 if(ret != OK)
 return ret;
 if(EXCEED_OFFSET(ps_bitstrm))
 return ERROR_EOB_TERMINATE_T;

  
 }
 else
 {
        ps_cur_mb_info->u1_qp_div6 = ps_dec->u1_qp_y_div6;
        ps_cur_mb_info->u1_qpc_div6 = ps_dec->u1_qp_u_div6;
        ps_cur_mb_info->u1_qpcr_div6 = ps_dec->u1_qp_v_div6;
        ps_cur_mb_info->u1_qp_rem6 = ps_dec->u1_qp_y_rem6;
        ps_cur_mb_info->u1_qpc_rem6 = ps_dec->u1_qp_u_rem6;
        ps_cur_mb_info->u1_qpcr_rem6 = ps_dec->u1_qp_v_rem6;
        ih264d_update_nnz_for_skipmb(ps_dec, ps_cur_mb_info, CAVLC);
 }

 return OK;
}
