WORD32 ih264d_parse_imb_cabac(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                              UWORD8 u1_mb_type)
{
    WORD8 i1_delta_qp;
    UWORD8 u1_cbp;
    UWORD8 u1_offset;
  
 ctxt_inc_mb_info_t *p_curr_ctxt = ps_dec->ps_curr_ctxt_mb_info;
 ctxt_inc_mb_info_t *ps_left_ctxt = ps_dec->p_left_ctxt_mb_info;
 dec_bit_stream_t * const ps_bitstrm = ps_dec->ps_bitstrm;
 bin_ctxt_model_t *p_bin_ctxt;

    UWORD8 u1_intra_chrom_pred_mode;
    UWORD8 u1_dc_block_flag = 0;
    WORD32 ret;

    ps_cur_mb_info->u1_yuv_dc_block_flag = 0;

 if(ps_left_ctxt == ps_dec->ps_def_ctxt_mb_info)
 {
        ps_dec->pu1_left_yuv_dc_csbp[0] = 0xf;
 }

 if(ps_dec->ps_cur_slice->u1_slice_type != I_SLICE)
 {
        WORD32 *pi4_buf;
        WORD8 *pi1_buf;
        MEMSET_16BYTES(&ps_dec->pu1_left_mv_ctxt_inc[0][0], 0);
 *((UWORD32 *)ps_dec->pi1_left_ref_idx_ctxt_inc) = 0;
        MEMSET_16BYTES(p_curr_ctxt->u1_mv, 0);
        pi1_buf = p_curr_ctxt->i1_ref_idx;
        pi4_buf = (WORD32 *)pi1_buf;
 *pi4_buf = 0;
 }

 if(u1_mb_type == I_4x4_MB)
 {
        ps_cur_mb_info->ps_curmb->u1_mb_type = I_4x4_MB;
        p_curr_ctxt->u1_mb_type = CAB_I4x4;
        u1_offset = 0;

        ps_cur_mb_info->u1_tran_form8x8 = 0;
        ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

  
  
  
 if(ps_dec->s_high_profile.u1_transform8x8_present)
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

  
  
  
 if (!ps_cur_mb_info->u1_tran_form8x8)
 {
            UWORD8 *pu1_temp;
            ih264d_read_intra_pred_modes_cabac(
                            ps_dec,
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
            ih264d_read_intra_pred_modes_cabac(
                            ps_dec,
 ((UWORD8 *)ps_dec->pv_parse_tu_coeff_data),
 ((UWORD8 *)ps_dec->pv_parse_tu_coeff_data+4),
                            ps_cur_mb_info->u1_tran_form8x8);
            pu1_temp = (UWORD8 *)ps_dec->pv_parse_tu_coeff_data;
            pu1_temp += 8;
            ps_dec->pv_parse_tu_coeff_data = (void *)pu1_temp;
 }
  
  
  
        u1_intra_chrom_pred_mode = ih264d_parse_chroma_pred_mode_cabac(ps_dec);
        COPYTHECONTEXT("intra_chroma_pred_mode", u1_intra_chrom_pred_mode);
        p_curr_ctxt->u1_intra_chroma_pred_mode = ps_cur_mb_info->u1_chroma_pred_mode =
                        u1_intra_chrom_pred_mode;

  
  
  
        u1_cbp = ih264d_parse_ctx_cbp_cabac(ps_dec);
        COPYTHECONTEXT("coded_block_pattern", u1_cbp);
        ps_cur_mb_info->u1_cbp = u1_cbp;
        p_curr_ctxt->u1_cbp = u1_cbp;

  
  
  
 if(ps_cur_mb_info->u1_cbp)
 {
            ret = ih264d_parse_mb_qp_delta_cabac(ps_dec, &i1_delta_qp);
 if(ret != OK)
 return ret;
            COPYTHECONTEXT("mb_qp_delta", i1_delta_qp);
 if(i1_delta_qp != 0)
 {
                ret = ih264d_update_qp(ps_dec, i1_delta_qp);
 if(ret != OK)
 return ret;
 }
 }
 else
            ps_dec->i1_prev_mb_qp_delta = 0;
        p_curr_ctxt->u1_yuv_dc_csbp &= 0xFE;
 }
 else
 {
        u1_offset = 1;
        ps_cur_mb_info->ps_curmb->u1_mb_type = I_16x16_MB;
        p_curr_ctxt->u1_mb_type = CAB_I16x16;
        ps_cur_mb_info->u1_tran_form8x8 = 0;
        p_curr_ctxt->u1_transform8x8_ctxt = 0;
        ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;
  
  
  
        u1_intra_chrom_pred_mode = ih264d_parse_chroma_pred_mode_cabac(ps_dec);
 if(u1_intra_chrom_pred_mode > 3)
 return ERROR_CHROMA_PRED_MODE;

        COPYTHECONTEXT("Chroma intra_chroma_pred_mode pred mode", u1_intra_chrom_pred_mode);
        p_curr_ctxt->u1_intra_chroma_pred_mode = ps_cur_mb_info->u1_chroma_pred_mode =
                        u1_intra_chrom_pred_mode;

  
  
  
        u1_cbp = gau1_ih264d_cbp_tab[(u1_mb_type - 1) >> 2];
        ps_cur_mb_info->u1_cbp = u1_cbp;
        p_curr_ctxt->u1_cbp = u1_cbp;

  
  
  
        ret = ih264d_parse_mb_qp_delta_cabac(ps_dec, &i1_delta_qp);
 if(ret != OK)
 return ret;
        COPYTHECONTEXT("mb_qp_delta", i1_delta_qp);
 if(i1_delta_qp != 0)
 {
            ret = ih264d_update_qp(ps_dec, i1_delta_qp);
 if(ret != OK)
 return ret;
 }

 {
            WORD16 i_scaleFactor;
            WORD16* pi2_scale_matrix_ptr;
  
  
  
  
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
 {
 ctxt_inc_mb_info_t *ps_top_ctxt = ps_dec->p_top_ctxt_mb_info;
                UWORD8 uc_a, uc_b;
                UWORD32 u4_ctx_inc;

                INC_SYM_COUNT(&(ps_dec->s_cab_dec_env));

  
                uc_b = ((ps_top_ctxt->u1_yuv_dc_csbp) & 0x01);

  
                uc_a = ((ps_dec->pu1_left_yuv_dc_csbp[0]) & 0x01);

                u4_ctx_inc = (uc_a + (uc_b << 1));

 {
                    WORD16 pi2_dc_coef[16];
 tu_sblk4x4_coeff_data_t *ps_tu_4x4 =
 (tu_sblk4x4_coeff_data_t *)ps_dec->pv_parse_tu_coeff_data;
                    WORD16 *pi2_coeff_block =
 (WORD16 *)ps_dec->pv_parse_tu_coeff_data;

                    p_bin_ctxt = (ps_dec->p_cbf_t[LUMA_DC_CTXCAT]) + u4_ctx_inc;

                    u1_dc_block_flag =
                                    ih264d_read_coeff4x4_cabac(ps_bitstrm,
                                                    LUMA_DC_CTXCAT,
                                                    ps_dec->p_significant_coeff_flag_t[LUMA_DC_CTXCAT],
                                                    ps_dec, p_bin_ctxt);

  
                    p_curr_ctxt->u1_yuv_dc_csbp &= 0xFE;
                    p_curr_ctxt->u1_yuv_dc_csbp |= u1_dc_block_flag;
 if(u1_dc_block_flag)
 {
                        WORD32 pi4_tmp[16];
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
 }

    ps_dec->pu1_left_yuv_dc_csbp[0] &= 0x6;
    ps_dec->pu1_left_yuv_dc_csbp[0] |= u1_dc_block_flag;

    ih264d_parse_residual4x4_cabac(ps_dec, ps_cur_mb_info, u1_offset);
 if(EXCEED_OFFSET(ps_bitstrm))
 return ERROR_EOB_TERMINATE_T;
 return OK;
}
