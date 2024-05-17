IHEVCD_ERROR_T  ihevcd_parse_coding_unit(codec_t *ps_codec,
                                         WORD32 x0,
                                         WORD32 y0,
                                         WORD32 log2_cb_size)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 sps_t *ps_sps;
 pps_t *ps_pps;
    WORD32 cb_size;
 slice_header_t *ps_slice_hdr;
    WORD32 skip_flag;
    WORD32 pcm_flag;
    UWORD32 *pu4_skip_top = ps_codec->s_parse.pu4_skip_cu_top;
    UWORD32 u4_skip_left = ps_codec->s_parse.u4_skip_cu_left;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
 tu_t *ps_tu = ps_codec->s_parse.ps_tu;

    WORD32 cu_pos_x;
    WORD32 cu_pos_y;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;

    ASSERT(0 == (x0 % 8));
    ASSERT(0 == (y0 % 8));

    ps_codec->s_parse.s_cu.i4_tu_cnt = 0;
    ps_sps = ps_codec->s_parse.ps_sps;
    ps_pps = ps_codec->s_parse.ps_pps;

    cu_pos_x = ps_codec->s_parse.s_cu.i4_pos_x;
    cu_pos_y = ps_codec->s_parse.s_cu.i4_pos_y;



    ps_slice_hdr = ps_codec->s_parse.ps_slice_hdr;


    cb_size = 1 << log2_cb_size;

    ps_codec->s_parse.s_cu.i4_cu_transquant_bypass = 0;

 if(ps_pps->i1_transquant_bypass_enable_flag)
 {
        TRACE_CABAC_CTXT("cu_transquant_bypass_flag", ps_cabac->u4_range, IHEVC_CAB_CU_TQ_BYPASS_FLAG);
        ps_codec->s_parse.s_cu.i4_cu_transquant_bypass =
                        ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm,
                                                IHEVC_CAB_CU_TQ_BYPASS_FLAG);
  

        AEV_TRACE("cu_transquant_bypass_flag", ps_codec->s_parse.s_cu.i4_cu_transquant_bypass,
                  ps_cabac->u4_range);

 if(ps_codec->s_parse.s_cu.i4_cu_transquant_bypass)
 {
            UWORD8 *pu1_pic_no_loop_filter_flag = ps_codec->s_parse.pu1_pic_no_loop_filter_flag;
            UWORD32 u4_mask;
            WORD32 i;
            WORD32 numbytes_row;
            numbytes_row = (ps_sps->i2_pic_width_in_luma_samples + 63) / 64;
            pu1_pic_no_loop_filter_flag += (y0 / 8) * numbytes_row;
            pu1_pic_no_loop_filter_flag += (x0 / 64);

  
  
            u4_mask = LSB_ONES((cb_size >> 3));
 for(i = 0; i < (cb_size / 8); i++)
 {
 *pu1_pic_no_loop_filter_flag |= (u4_mask << (((x0) / 8) % 8));
                pu1_pic_no_loop_filter_flag += numbytes_row;
 }
 }
 }

 {
        UWORD32 u4_skip_top = 0;
        UWORD32 u4_mask;
        UWORD32 u4_top_mask, u4_left_mask;
        UWORD32 u4_min_cu_x = x0 / 8;
        UWORD32 u4_min_cu_y = y0 / 8;

        pu4_skip_top += (u4_min_cu_x / 32);


 if(ps_slice_hdr->i1_slice_type != ISLICE)
 {
            WORD32 ctx_idx_inc;
            ctx_idx_inc = 0;

 if((0 != cu_pos_y) ||
 ((0 != ps_codec->s_parse.i4_ctb_slice_y) &&
 (0 != ps_codec->s_parse.i4_ctb_tile_y)))
 {
                u4_skip_top = *pu4_skip_top;
                u4_skip_top >>= (u4_min_cu_x % 32);
 if(u4_skip_top & 1)
                    ctx_idx_inc++;
 }

  
  
  
  
  
  
 if((0 != cu_pos_x) ||
 (((0 != ps_codec->s_parse.i4_ctb_slice_x) || (0 != ps_codec->s_parse.i4_ctb_slice_y)) &&
 (0 != ps_codec->s_parse.i4_ctb_tile_x)))
 {
                u4_skip_left >>= (u4_min_cu_y % 32);
 if(u4_skip_left & 1)
                    ctx_idx_inc++;
 }
            TRACE_CABAC_CTXT("cu_skip_flag", ps_cabac->u4_range, (IHEVC_CAB_SKIP_FLAG + ctx_idx_inc));
            skip_flag = ihevcd_cabac_decode_bin(ps_cabac,
                                                ps_bitstrm,
 (IHEVC_CAB_SKIP_FLAG + ctx_idx_inc));

            AEV_TRACE("cu_skip_flag", skip_flag, ps_cabac->u4_range);
 }
 else
            skip_flag = 0;

  
        u4_skip_top = *pu4_skip_top;
  
  
        u4_mask = LSB_ONES(cb_size / 8);
        u4_top_mask = u4_mask << (u4_min_cu_x % 32);


 if(skip_flag)
 {
            u4_skip_top |= u4_top_mask;
 }
 else
 {
            u4_skip_top &= ~u4_top_mask;
 }
 *pu4_skip_top = u4_skip_top;

  
        u4_skip_left = ps_codec->s_parse.u4_skip_cu_left;
        u4_mask = LSB_ONES(cb_size / 8);
        u4_left_mask = u4_mask << (u4_min_cu_y % 32);

 if(skip_flag)
 {
            u4_skip_left |= u4_left_mask;
 }
 else
 {
            u4_skip_left &= ~u4_left_mask;
 }
        ps_codec->s_parse.u4_skip_cu_left = u4_skip_left;
 }
    ps_codec->s_parse.i4_cu_pcm_flag = 0;

 if(skip_flag)
 {
        WORD32 ctb_x_base;
        WORD32 ctb_y_base;

        ctb_x_base = ps_codec->s_parse.i4_ctb_x << ps_sps->i1_log2_ctb_size;
        ctb_y_base = ps_codec->s_parse.i4_ctb_y << ps_sps->i1_log2_ctb_size;

        ps_tu->b1_cb_cbf = 0;
        ps_tu->b1_cr_cbf = 0;
        ps_tu->b1_y_cbf = 0;
        ps_tu->b4_pos_x = ((x0 - ctb_x_base) >> 2);
        ps_tu->b4_pos_y = ((y0 - ctb_y_base) >> 2);
        ps_tu->b1_transquant_bypass = 0;
        ps_tu->b3_size = (log2_cb_size - 2);
        ps_tu->b7_qp = ps_codec->s_parse.u4_qp;
        ps_tu->b3_chroma_intra_mode_idx = INTRA_PRED_CHROMA_IDX_NONE;
        ps_tu->b6_luma_intra_mode   = INTRA_PRED_NONE;

  
 {
 if((ps_codec->s_parse.s_cu.i4_pos_x << 3) == (ps_tu->b4_pos_x << 2) &&
 (ps_codec->s_parse.s_cu.i4_pos_y << 3) == (ps_tu->b4_pos_y << 2))
 {
                ps_tu->b1_first_tu_in_cu = 1;
 }
 else
 {
                ps_tu->b1_first_tu_in_cu = 0;
 }
 }

        ps_codec->s_parse.ps_tu++;
        ps_codec->s_parse.s_cu.i4_tu_cnt++;
        ps_codec->s_parse.i4_pic_tu_idx++;

        ps_codec->s_parse.s_cu.i4_pred_mode = PRED_MODE_SKIP;
        ps_codec->s_parse.s_cu.i4_part_mode = PART_2Nx2N;
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
            ps_pu->b2_part_idx = 0;
            ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size, cb_size);
            STATS_UPDATE_PU_SKIP_SIZE(ps_pu);
 }
 }
 else
 {
        WORD32 pred_mode;
        WORD32 part_mode;
        WORD32 intra_split_flag;
        WORD32 is_mincb;
        cb_size = (1 << log2_cb_size);
        is_mincb = (cb_size == (1 << ps_sps->i1_log2_min_coding_block_size));
        pcm_flag = 0;
 if(ps_slice_hdr->i1_slice_type != ISLICE)
 {
            TRACE_CABAC_CTXT("pred_mode_flag", ps_cabac->u4_range, IHEVC_CAB_PRED_MODE);
            pred_mode = ihevcd_cabac_decode_bin(ps_cabac,
                                                ps_bitstrm,
                                                IHEVC_CAB_PRED_MODE);

            AEV_TRACE("pred_mode_flag", pred_mode, ps_cabac->u4_range);
 }
 else
 {
            pred_mode = PRED_MODE_INTRA;
 }

  
 if(PRED_MODE_INTRA == pred_mode)
 {
            UWORD8 *pu1_pic_intra_flag = ps_codec->s_parse.pu1_pic_intra_flag;
            UWORD32 u4_mask;
            WORD32 i;
            WORD32 numbytes_row;
            numbytes_row = (ps_sps->i2_pic_width_in_luma_samples + 63) / 64;
            pu1_pic_intra_flag += (y0 / 8) * numbytes_row;
            pu1_pic_intra_flag += (x0 / 64);

  
  
            u4_mask = LSB_ONES((cb_size >> 3));
 for(i = 0; i < (cb_size / 8); i++)
 {
 *pu1_pic_intra_flag |= (u4_mask << (((x0) / 8) % 8));
                pu1_pic_intra_flag += numbytes_row;
 }
 }

        ps_codec->s_parse.s_cu.i4_pred_mode = pred_mode;
        intra_split_flag = 0;
 if((PRED_MODE_INTRA != pred_mode) ||
                        is_mincb)
 {
            UWORD32 bin;
 if(PRED_MODE_INTRA == pred_mode)
 {
                TRACE_CABAC_CTXT("part_mode", ps_cabac->u4_range, IHEVC_CAB_PART_MODE);
                bin = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, IHEVC_CAB_PART_MODE);
                part_mode = (bin) ? PART_2Nx2N : PART_NxN;
 }
 else
 {
                WORD32 amp_enabled = ps_sps->i1_amp_enabled_flag;

                UWORD32 u4_max_bin_cnt = 0;



 if(amp_enabled && !is_mincb)
 {
                    part_mode = ihevcd_parse_part_mode_amp(ps_cabac, ps_bitstrm);
 }
 else
 {
                    WORD32 ctxt_inc = IHEVC_CAB_PART_MODE;

                    u4_max_bin_cnt = 2;
 if((is_mincb) && (cb_size > 8))
 {
                        u4_max_bin_cnt++;
 }

                    part_mode = -1;
                    TRACE_CABAC_CTXT("part_mode", ps_cabac->u4_range, IHEVC_CAB_PART_MODE);
 do
 {
                        bin = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm,
                                                      ctxt_inc++);
                        part_mode++;
 }while(--u4_max_bin_cnt && !bin);

  
 if(!bin)
                        part_mode++;
 }


 }

            AEV_TRACE("part_mode", part_mode, ps_cabac->u4_range);

 }
 else
 {
            part_mode = 0;
            intra_split_flag = 0;
 }
        ps_codec->s_parse.s_cu.i4_part_mode = part_mode;

 if((PRED_MODE_INTRA == ps_codec->s_parse.s_cu.i4_pred_mode) &&
 (PART_NxN == ps_codec->s_parse.s_cu.i4_part_mode))
 {
            intra_split_flag = 1;
 }
        ps_codec->s_parse.s_cu.i4_part_mode = part_mode;
        ps_codec->s_parse.s_cu.i4_intra_split_flag = intra_split_flag;
 if(pred_mode == PRED_MODE_INTRA)
 {
            ps_codec->s_parse.i4_cu_pcm_flag = 0;
            ihevcd_parse_coding_unit_intra(ps_codec, x0, y0, log2_cb_size);
            pcm_flag = ps_codec->s_parse.i4_cu_pcm_flag;

 }
 else
 {
 if(part_mode == PART_2Nx2N)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size, cb_size);
                ps_pu->b2_part_idx = 0;
 }
 else if(part_mode == PART_2NxN)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;

                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size, cb_size / 2);
                ps_pu->b2_part_idx = 0;

                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0 + (cb_size / 2), cb_size, cb_size / 2);

                ps_pu->b2_part_idx = 1;
 }
 else if(part_mode == PART_Nx2N)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size / 2, cb_size);
                ps_pu->b2_part_idx = 0;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0 + (cb_size / 2), y0, cb_size / 2, cb_size);

                ps_pu->b2_part_idx = 1;
 }
 else if(part_mode == PART_2NxnU)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size, cb_size / 4);
                ps_pu->b2_part_idx = 0;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0 + (cb_size / 4), cb_size, cb_size * 3 / 4);

                ps_pu->b2_part_idx = 1;
 }
 else if(part_mode == PART_2NxnD)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size, cb_size * 3 / 4);
                ps_pu->b2_part_idx = 0;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0 + (cb_size * 3 / 4), cb_size, cb_size / 4);

                ps_pu->b2_part_idx = 1;
 }
 else if(part_mode == PART_nLx2N)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size / 4, cb_size);
                ps_pu->b2_part_idx = 0;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0 + (cb_size / 4), y0, cb_size * 3 / 4, cb_size);

                ps_pu->b2_part_idx = 1;
 }
 else if(part_mode == PART_nRx2N)
 {
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size * 3 / 4, cb_size);
                ps_pu->b2_part_idx = 0;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0 + (cb_size * 3 / 4), y0, cb_size / 4, cb_size);
                ps_pu->b2_part_idx = 1;
 }
 else
 {  
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;

                ihevcd_parse_prediction_unit(ps_codec, x0, y0, cb_size / 2, cb_size / 2);
                ps_pu->b2_part_idx = 0;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0 + (cb_size / 2), y0, cb_size / 2, cb_size / 2);

                ps_pu->b2_part_idx = 1;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0, y0 + (cb_size / 2), cb_size / 2, cb_size / 2);

                ps_pu->b2_part_idx = 2;
                ps_pu = ps_codec->s_parse.ps_pu;
                ihevcd_parse_prediction_unit(ps_codec, x0 + (cb_size / 2), y0 + (cb_size / 2), cb_size / 2, cb_size / 2);

                ps_pu->b2_part_idx = 3;
 }
 }

 if(!pcm_flag)
 {
            WORD32 no_residual_syntax_flag = 0;
 pu_t *ps_pu;
  
            ps_pu = ps_codec->s_parse.ps_pu - 1;
 if((PRED_MODE_INTRA != pred_mode) &&
 (!((part_mode == PART_2Nx2N) && ps_pu->b1_merge_flag)))
 {

                TRACE_CABAC_CTXT("rqt_root_cbf", ps_cabac->u4_range, IHEVC_CAB_NORES_IDX);
                no_residual_syntax_flag = ihevcd_cabac_decode_bin(ps_cabac,
                                                                  ps_bitstrm,
                                                                  IHEVC_CAB_NORES_IDX);

                AEV_TRACE("rqt_root_cbf", no_residual_syntax_flag,
                          ps_cabac->u4_range);
  
  
  
  
  
  
                no_residual_syntax_flag = 1 - no_residual_syntax_flag;
 }

 if(!no_residual_syntax_flag)
 {

                ps_codec->s_parse.s_cu.i4_max_trafo_depth = (pred_mode == PRED_MODE_INTRA) ?
 (ps_sps->i1_max_transform_hierarchy_depth_intra + intra_split_flag) :
 (ps_sps->i1_max_transform_hierarchy_depth_inter);
                ihevcd_parse_transform_tree(ps_codec, x0, y0, x0, y0,
                                            log2_cb_size, 0, 0,
                                            ps_codec->s_parse.s_cu.ai4_intra_luma_pred_mode[0]);
 }
 else
 {
                WORD32 ctb_x_base;
                WORD32 ctb_y_base;

                ctb_x_base = ps_codec->s_parse.i4_ctb_x << ps_sps->i1_log2_ctb_size;
                ctb_y_base = ps_codec->s_parse.i4_ctb_y << ps_sps->i1_log2_ctb_size;

                ps_tu = ps_codec->s_parse.ps_tu;
                ps_tu->b1_cb_cbf = 0;
                ps_tu->b1_cr_cbf = 0;
                ps_tu->b1_y_cbf = 0;
                ps_tu->b4_pos_x = ((x0 - ctb_x_base) >> 2);
                ps_tu->b4_pos_y = ((y0 - ctb_y_base) >> 2);
                ps_tu->b1_transquant_bypass = 0;
                ps_tu->b3_size = (log2_cb_size - 2);
                ps_tu->b7_qp = ps_codec->s_parse.u4_qp;
                ps_tu->b3_chroma_intra_mode_idx = INTRA_PRED_CHROMA_IDX_NONE;
                ps_tu->b6_luma_intra_mode   = ps_codec->s_parse.s_cu.ai4_intra_luma_pred_mode[0];

  
 {
 if((ps_codec->s_parse.s_cu.i4_pos_x << 3) == (ps_tu->b4_pos_x << 2) &&
 (ps_codec->s_parse.s_cu.i4_pos_y << 3) == (ps_tu->b4_pos_y << 2))
 {
                        ps_tu->b1_first_tu_in_cu = 1;
 }
 else
 {
                        ps_tu->b1_first_tu_in_cu = 0;
 }
 }
                ps_codec->s_parse.ps_tu++;
                ps_codec->s_parse.s_cu.i4_tu_cnt++;
                ps_codec->s_parse.i4_pic_tu_idx++;

 }
 }

 }




 return ret;
}
