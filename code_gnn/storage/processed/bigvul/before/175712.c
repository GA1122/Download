WORD16 ih264d_allocate_dynamic_bufs(dec_struct_t * ps_dec)
{
 struct MemReq s_MemReq;
 struct MemBlock *p_MemBlock;

 pred_info_t *ps_pred_frame;
 dec_mb_info_t *ps_frm_mb_info;
 dec_slice_struct_t *ps_dec_slice_buf;
    UWORD8 *pu1_dec_mb_map, *pu1_recon_mb_map;
    UWORD16 *pu2_slice_num_map;

    WORD16 *pi16_res_coeff;
    WORD16 i16_status = 0;
    UWORD8 uc_frmOrFld = (1 - ps_dec->ps_cur_sps->u1_frame_mbs_only_flag);
    UWORD16 u4_luma_wd = ps_dec->u2_frm_wd_y;
    UWORD16 u4_chroma_wd = ps_dec->u2_frm_wd_uv;
    WORD8 c_i = 0;
 dec_seq_params_t *ps_sps = ps_dec->ps_cur_sps;
    UWORD32 u4_total_mbs = ps_sps->u2_total_num_of_mbs << uc_frmOrFld;
    UWORD32 u4_wd_mbs = ps_dec->u2_frm_wd_in_mbs;
    UWORD32 u4_ht_mbs = ps_dec->u2_frm_ht_in_mbs;
    UWORD32 u4_blk_wd;
    UWORD32 ui_size = 0;
    UWORD32 u4_int_scratch_size = 0, u4_ref_pred_size = 0;
    UWORD8 *pu1_buf;
    WORD32 num_entries;
    WORD32 size;
 void *pv_buf;
    UWORD32 u4_num_bufs;
    UWORD32 u4_luma_size, u4_chroma_size;
 void *pv_mem_ctxt = ps_dec->pv_mem_ctxt;

    size = u4_total_mbs;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_dec_mb_map = pv_buf;

    size = u4_total_mbs;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_recon_mb_map = pv_buf;

    size = u4_total_mbs * sizeof(UWORD16);
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu2_slice_num_map = pv_buf;

  
  
  
    ps_dec->ps_parse_cur_slice = &(ps_dec->ps_dec_slice_buf[0]);
    ps_dec->ps_decode_cur_slice = &(ps_dec->ps_dec_slice_buf[0]);
    ps_dec->ps_computebs_cur_slice = &(ps_dec->ps_dec_slice_buf[0]);

    ps_dec->ps_pred_start = ps_dec->ps_pred;

    size = sizeof(parse_pmbarams_t) * (ps_dec->u1_recon_mb_grp);
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_parse_mb_data = pv_buf;

    size = sizeof(parse_part_params_t)
 * ((ps_dec->u1_recon_mb_grp) << 4);
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_parse_part_params = pv_buf;

    size = ((u4_wd_mbs * sizeof(deblkmb_neighbour_t)) << uc_frmOrFld);
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_deblk_top_mb = pv_buf;

    size = ((sizeof(ctxt_inc_mb_info_t))
 * (((u4_wd_mbs + 1) << uc_frmOrFld) + 1));
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->p_ctxt_inc_mb_map = pv_buf;

    size = (sizeof(mv_pred_t) * ps_dec->u1_recon_mb_grp
 * 16);
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_mv_p[0] = pv_buf;

    size = (sizeof(mv_pred_t) * ps_dec->u1_recon_mb_grp
 * 16);
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_mv_p[1] = pv_buf;

 {
        UWORD8 i;
 for(i = 0; i < MV_SCRATCH_BUFS; i++)
 {
            size = (sizeof(mv_pred_t)
 * ps_dec->u1_recon_mb_grp * 4);
            pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
            RETURN_IF((NULL == pv_buf), IV_FAIL);
            ps_dec->ps_mv_top_p[i] = pv_buf;
 }
 }

    size = sizeof(UWORD8) * ((u4_wd_mbs + 2) * MB_SIZE) * 2;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_y_intra_pred_line = pv_buf;
    memset(ps_dec->pu1_y_intra_pred_line, 0, size);
    ps_dec->pu1_y_intra_pred_line += MB_SIZE;

    size = sizeof(UWORD8) * ((u4_wd_mbs + 2) * MB_SIZE) * 2;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_u_intra_pred_line = pv_buf;
    memset(ps_dec->pu1_u_intra_pred_line, 0, size);
    ps_dec->pu1_u_intra_pred_line += MB_SIZE;

    size = sizeof(UWORD8) * ((u4_wd_mbs + 2) * MB_SIZE) * 2;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_v_intra_pred_line = pv_buf;
    memset(ps_dec->pu1_v_intra_pred_line, 0, size);
    ps_dec->pu1_v_intra_pred_line += MB_SIZE;

 if(ps_dec->u1_separate_parse)
 {
  
        size = sizeof(mb_neigbour_params_t)
 * 2 * ((u4_wd_mbs + 2) * (u4_ht_mbs + 1));
 }
 else
 {
        size = sizeof(mb_neigbour_params_t)
 * 2 * ((u4_wd_mbs + 2) << uc_frmOrFld);
 }
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);

    ps_dec->ps_nbr_mb_row = pv_buf;
    memset(ps_dec->ps_nbr_mb_row, 0, size);

  
    size = (u4_total_mbs + u4_wd_mbs) * sizeof(deblk_mb_t);

    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_deblk_pic = pv_buf;

    memset(ps_dec->ps_deblk_pic, 0, size);

  
    size = sizeof(dec_mb_info_t) * u4_total_mbs;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_frm_mb_info = pv_buf;
    memset(ps_dec->ps_frm_mb_info, 0, size);

  
    num_entries = MAX_FRAMES;
 if((1 >= ps_dec->ps_cur_sps->u1_num_ref_frames) &&
 (0 == ps_dec->i4_display_delay))
 {
        num_entries = 1;
 }
    num_entries = ((2 * num_entries) + 1);
 if(BASE_PROFILE_IDC != ps_dec->ps_cur_sps->u1_profile_idc)
 {
        num_entries *= 2;
 }

    size = num_entries * sizeof(void *);
    size += PAD_MAP_IDX_POC * sizeof(void *);
    size *= u4_total_mbs;
    size += sizeof(dec_slice_struct_t) * u4_total_mbs;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);

    ps_dec->ps_dec_slice_buf = pv_buf;
    memset(ps_dec->ps_dec_slice_buf, 0, size);
    pu1_buf = (UWORD8 *)ps_dec->ps_dec_slice_buf;
    pu1_buf += sizeof(dec_slice_struct_t) * u4_total_mbs;
    ps_dec->pv_map_ref_idx_to_poc_buf = (void *)pu1_buf;

  
    num_entries = u4_total_mbs;
 if(1 == ps_dec->ps_cur_sps->u1_num_ref_frames)
        num_entries *= 16;
 else
        num_entries *= 16 * 2;

    size = sizeof(pred_info_pkd_t) * num_entries;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_pred_pkd = pv_buf;

  
    size = MB_LUM_SIZE * sizeof(WORD16);
  
    size += u4_total_mbs * (MAX(17 * sizeof(tu_sblk4x4_coeff_data_t),4 * sizeof(tu_blk8x8_coeff_data_t))
 + 9 * sizeof(tu_sblk4x4_coeff_data_t));
    size += u4_total_mbs * 32;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);

    ps_dec->pi2_coeff_data = pv_buf;

    ps_dec->pv_pic_tu_coeff_data = (void *)(ps_dec->pi2_coeff_data + MB_LUM_SIZE);

  
 {
        UWORD32 col_flag_buffer_size, mvpred_buffer_size;

        col_flag_buffer_size = ((ps_dec->u2_pic_wd * ps_dec->u2_pic_ht) >> 4);
        mvpred_buffer_size = sizeof(mv_pred_t)
 * ((ps_dec->u2_pic_wd * (ps_dec->u2_pic_ht + PAD_MV_BANK_ROW)) >> 4);

        u4_num_bufs = ps_dec->ps_cur_sps->u1_num_ref_frames + 1;

        u4_num_bufs = MIN(u4_num_bufs, ps_dec->u1_pic_bufs);
        u4_num_bufs = MAX(u4_num_bufs, 2);
        size = ALIGN64(mvpred_buffer_size) + ALIGN64(col_flag_buffer_size);
        size *= u4_num_bufs;
        pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
        RETURN_IF((NULL == pv_buf), IV_FAIL);
        ps_dec->pu1_mv_bank_buf_base = pv_buf;
 }

  
    u4_luma_size = ps_dec->u2_frm_wd_y * ps_dec->u2_frm_ht_y;
    u4_chroma_size = ps_dec->u2_frm_wd_uv * ps_dec->u2_frm_ht_uv;

 {
 if(ps_dec->u4_share_disp_buf == 1)
 {
  
 if((ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 || (ps_dec->u1_chroma_format == IV_YUV_420SP_VU)
 || (ps_dec->u1_chroma_format == IV_YUV_420P))
 {
                u4_luma_size = 64;
 }

 if(ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 {
                u4_chroma_size = 64;
 }

 }
 }

    size = ALIGN64(u4_luma_size) + ALIGN64(u4_chroma_size);
    size *= ps_dec->u1_pic_bufs;
    pv_buf = ps_dec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_pic_buf_base = pv_buf;

  
    ps_dec->p_ctxt_inc_mb_map += 1;
  

  
  
  
 {
 bin_ctxt_model_t * const p_cabac_ctxt_table_t =
                        ps_dec->p_cabac_ctxt_table_t;
 bin_ctxt_model_t * * p_coeff_abs_level_minus1_t =
                        ps_dec->p_coeff_abs_level_minus1_t;
 bin_ctxt_model_t * * p_cbf_t = ps_dec->p_cbf_t;

        ps_dec->p_mb_field_dec_flag_t = p_cabac_ctxt_table_t
 + MB_FIELD_DECODING_FLAG;
        ps_dec->p_prev_intra4x4_pred_mode_flag_t = p_cabac_ctxt_table_t
 + PREV_INTRA4X4_PRED_MODE_FLAG;
        ps_dec->p_rem_intra4x4_pred_mode_t = p_cabac_ctxt_table_t
 + REM_INTRA4X4_PRED_MODE;
        ps_dec->p_intra_chroma_pred_mode_t = p_cabac_ctxt_table_t
 + INTRA_CHROMA_PRED_MODE;
        ps_dec->p_mb_qp_delta_t = p_cabac_ctxt_table_t + MB_QP_DELTA;
        ps_dec->p_ref_idx_t = p_cabac_ctxt_table_t + REF_IDX;
        ps_dec->p_mvd_x_t = p_cabac_ctxt_table_t + MVD_X;
        ps_dec->p_mvd_y_t = p_cabac_ctxt_table_t + MVD_Y;
 p_cbf_t[0] = p_cabac_ctxt_table_t + CBF + 0;
 p_cbf_t[1] = p_cabac_ctxt_table_t + CBF + 4;
 p_cbf_t[2] = p_cabac_ctxt_table_t + CBF + 8;
 p_cbf_t[3] = p_cabac_ctxt_table_t + CBF + 12;
 p_cbf_t[4] = p_cabac_ctxt_table_t + CBF + 16;
        ps_dec->p_cbp_luma_t = p_cabac_ctxt_table_t + CBP_LUMA;
        ps_dec->p_cbp_chroma_t = p_cabac_ctxt_table_t + CBP_CHROMA;

 p_coeff_abs_level_minus1_t[LUMA_DC_CTXCAT] = p_cabac_ctxt_table_t
 + COEFF_ABS_LEVEL_MINUS1 + COEFF_ABS_LEVEL_CAT_0_OFFSET;

 p_coeff_abs_level_minus1_t[LUMA_AC_CTXCAT] = p_cabac_ctxt_table_t
 + COEFF_ABS_LEVEL_MINUS1 + COEFF_ABS_LEVEL_CAT_1_OFFSET;

 p_coeff_abs_level_minus1_t[LUMA_4X4_CTXCAT] = p_cabac_ctxt_table_t
 + COEFF_ABS_LEVEL_MINUS1 + COEFF_ABS_LEVEL_CAT_2_OFFSET;

 p_coeff_abs_level_minus1_t[CHROMA_DC_CTXCAT] = p_cabac_ctxt_table_t
 + COEFF_ABS_LEVEL_MINUS1 + COEFF_ABS_LEVEL_CAT_3_OFFSET;

 p_coeff_abs_level_minus1_t[CHROMA_AC_CTXCAT] = p_cabac_ctxt_table_t
 + COEFF_ABS_LEVEL_MINUS1 + COEFF_ABS_LEVEL_CAT_4_OFFSET;

 p_coeff_abs_level_minus1_t[LUMA_8X8_CTXCAT] = p_cabac_ctxt_table_t
 + COEFF_ABS_LEVEL_MINUS1_8X8
 + COEFF_ABS_LEVEL_CAT_5_OFFSET;

  
  
  
  
 {

            ps_dec->s_high_profile.ps_transform8x8_flag = p_cabac_ctxt_table_t
 + TRANSFORM_SIZE_8X8_FLAG;

            ps_dec->s_high_profile.ps_sigcoeff_8x8_frame = p_cabac_ctxt_table_t
 + SIGNIFICANT_COEFF_FLAG_8X8_FRAME;

            ps_dec->s_high_profile.ps_last_sigcoeff_8x8_frame =
 p_cabac_ctxt_table_t
 + LAST_SIGNIFICANT_COEFF_FLAG_8X8_FRAME;

            ps_dec->s_high_profile.ps_coeff_abs_levelminus1 =
 p_cabac_ctxt_table_t + COEFF_ABS_LEVEL_MINUS1_8X8;

            ps_dec->s_high_profile.ps_sigcoeff_8x8_field = p_cabac_ctxt_table_t
 + SIGNIFICANT_COEFF_FLAG_8X8_FIELD;

            ps_dec->s_high_profile.ps_last_sigcoeff_8x8_field =
 p_cabac_ctxt_table_t
 + LAST_SIGNIFICANT_COEFF_FLAG_8X8_FIELD;
 }
 }
 return (i16_status);
}
