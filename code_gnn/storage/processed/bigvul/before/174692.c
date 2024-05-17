WORD32 ih264d_parse_bmb_cabac(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                              UWORD8 u1_mb_num,
                              UWORD8 u1_num_mbsNby2)
{
    UWORD8 u1_cbp;
 deblk_mb_t * ps_cur_deblk_mb = ps_dec->ps_deblk_mbn + u1_mb_num;
 const UWORD8 *puc_mb_mc_mode = (const UWORD8 *)gau1_ih264d_mb_mc_mode;
    UWORD8 u1_mb_type = ps_cur_mb_info->u1_mb_type;
 ctxt_inc_mb_info_t *p_curr_ctxt = ps_dec->ps_curr_ctxt_mb_info;

    WORD32 ret;
    UWORD8 u1_Bdirect_tranform_read = 1;
    ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag = 1;

    ps_cur_mb_info->u1_mb_mc_mode = puc_mb_mc_mode[5 + u1_mb_type];

    ps_cur_mb_info->u1_yuv_dc_block_flag = 0;

    ps_cur_deblk_mb->u1_mb_type |= D_B_SLICE;
 if(u1_mb_type != B_DIRECT)
 {
        ret = ih264d_parse_bmb_non_direct_cabac(ps_dec, ps_cur_mb_info, u1_mb_num,
                                          u1_num_mbsNby2);
 if(ret != OK)
 return ret;
 }
 else
 {

  
 parse_part_params_t * ps_part_info;
        ps_part_info = ps_dec->ps_part;
        ps_part_info->u1_is_direct = PART_DIRECT_16x16;
        ps_part_info->u1_sub_mb_num = 0;
        ps_dec->ps_part++;
        p_curr_ctxt->u1_mb_type = CAB_BD16x16;

        MEMSET_16BYTES(&ps_dec->pu1_left_mv_ctxt_inc[0][0], 0);
        memset(ps_dec->pi1_left_ref_idx_ctxt_inc, 0, 4);
        MEMSET_16BYTES(p_curr_ctxt->u1_mv, 0);
        memset(p_curr_ctxt->i1_ref_idx, 0, 4);

  
        u1_Bdirect_tranform_read =
                        ps_dec->s_high_profile.u1_direct_8x8_inference_flag;
 }

  
    u1_cbp = (WORD8)ih264d_parse_ctx_cbp_cabac(ps_dec);
    p_curr_ctxt->u1_cbp = u1_cbp;
    ps_cur_mb_info->u1_cbp = u1_cbp;

 if(u1_cbp > 47)
 return ERROR_CBP;

    COPYTHECONTEXT("coded_block_pattern", u1_cbp);

    ps_cur_mb_info->u1_tran_form8x8 = 0;
    ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = 0;

 if((ps_dec->s_high_profile.u1_transform8x8_present) && (u1_cbp & (0xf))
 && (ps_dec->s_high_profile.u1_no_submb_part_size_lt8x8_flag)
 && (u1_Bdirect_tranform_read))
 {
        ps_cur_mb_info->u1_tran_form8x8 = ih264d_parse_transform8x8flag_cabac(
                        ps_dec, ps_cur_mb_info);
        COPYTHECONTEXT("transform_size_8x8_flag", ps_cur_mb_info->u1_tran_form8x8);

        ps_cur_mb_info->ps_curmb->u1_tran_form8x8 = ps_cur_mb_info->u1_tran_form8x8;
        p_curr_ctxt->u1_transform8x8_ctxt = ps_cur_mb_info->u1_tran_form8x8;
 }
 else
 {
        p_curr_ctxt->u1_transform8x8_ctxt = 0;
 }

    p_curr_ctxt->u1_intra_chroma_pred_mode = 0;
    p_curr_ctxt->u1_yuv_dc_csbp &= 0xFE;
    ps_dec->pu1_left_yuv_dc_csbp[0] &= 0x6;

  
 if(u1_cbp)
 {
        WORD8 c_temp;
        ret = ih264d_parse_mb_qp_delta_cabac(ps_dec, &c_temp);
 if(ret != OK)
 return ret;
        COPYTHECONTEXT("mb_qp_delta", c_temp);
 if(c_temp)
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
 return OK;
}
