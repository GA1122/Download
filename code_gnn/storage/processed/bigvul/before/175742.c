IHEVCD_ERROR_T ihevcd_parse_coding_unit_intra(codec_t *ps_codec,
                                              WORD32 x0,
                                              WORD32 y0,
                                              WORD32 log2_cb_size)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 sps_t *ps_sps;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
    WORD32 pcm_flag;
    WORD32 value;
    WORD32 cb_size = 1 << log2_cb_size;
    WORD32 part_mode =  ps_codec->s_parse.s_cu.i4_part_mode;
 tu_t *ps_tu = ps_codec->s_parse.ps_tu;
 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
    WORD32 ctb_x_base;
    WORD32 ctb_y_base;
    ps_sps = ps_codec->s_parse.ps_sps;
    ctb_x_base = ps_codec->s_parse.i4_ctb_x << ps_sps->i1_log2_ctb_size;
    ctb_y_base = ps_codec->s_parse.i4_ctb_y << ps_sps->i1_log2_ctb_size;

    memset(ps_pu, 0, sizeof(pu_t));
    ps_pu->b1_intra_flag = 1;
    ps_pu->b4_wd = (cb_size >> 2) - 1;
    ps_pu->b4_ht = (cb_size >> 2) - 1;
    ps_pu->b4_pos_x = (x0 - ctb_x_base) >> 2;
    ps_pu->b4_pos_y = (y0 - ctb_y_base) >> 2;

    pcm_flag = 0;
 if((PART_2Nx2N == part_mode) && (ps_sps->i1_pcm_enabled_flag)
 && (log2_cb_size
 >= ps_sps->i1_log2_min_pcm_coding_block_size)
 && (log2_cb_size
 <= (ps_sps->i1_log2_min_pcm_coding_block_size + ps_sps->i1_log2_diff_max_min_pcm_coding_block_size)))
 {
        TRACE_CABAC_CTXT("pcm_flag", ps_cabac->u4_range, 0);
        pcm_flag = ihevcd_cabac_decode_terminate(ps_cabac, ps_bitstrm);
        AEV_TRACE("pcm_flag", pcm_flag, ps_cabac->u4_range);
 }

    ps_codec->s_parse.i4_cu_pcm_flag = pcm_flag;
 if(pcm_flag)
 {
        UWORD8 *pu1_luma_intra_pred_mode_top, *pu1_luma_intra_pred_mode_left;
        WORD32 i,  num_pred_blocks;

 if(ps_codec->s_parse.s_bitstrm.u4_bit_ofst % 8)
 {
            TRACE_CABAC_CTXT("pcm_alignment_zero_bit", ps_cabac->u4_range, 0);
            ihevcd_bits_flush_to_byte_boundary(&ps_codec->s_parse.s_bitstrm);
            AEV_TRACE("pcm_alignment_zero_bit", 0, ps_cabac->u4_range);
 }

        ihevcd_parse_pcm_sample(ps_codec, x0, y0, log2_cb_size);

        ihevcd_cabac_reset(&ps_codec->s_parse.s_cabac,
 &ps_codec->s_parse.s_bitstrm);

        ps_tu = ps_codec->s_parse.ps_tu;
        ps_tu->b1_cb_cbf = 1;
        ps_tu->b1_cr_cbf = 1;
        ps_tu->b1_y_cbf = 1;
        ps_tu->b4_pos_x = ((x0 - ctb_x_base) >> 2);
        ps_tu->b4_pos_y = ((y0 - ctb_y_base) >> 2);
        ps_tu->b1_transquant_bypass = 1;
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

  
        pu1_luma_intra_pred_mode_top = ps_codec->s_parse.pu1_luma_intra_pred_mode_top
 + (ps_codec->s_parse.s_cu.i4_pos_x * 2);

        pu1_luma_intra_pred_mode_left = ps_codec->s_parse.pu1_luma_intra_pred_mode_left
 + (ps_codec->s_parse.s_cu.i4_pos_y * 2);

        num_pred_blocks = 1;  

        ps_codec->s_func_selector.ihevc_memset_fptr(pu1_luma_intra_pred_mode_left, INTRA_DC, (cb_size / num_pred_blocks) / MIN_PU_SIZE);
        ps_codec->s_func_selector.ihevc_memset_fptr(pu1_luma_intra_pred_mode_top, INTRA_DC, (cb_size / num_pred_blocks) / MIN_PU_SIZE);


  
 if(1 == ps_sps->i1_pcm_loop_filter_disable_flag)
 {
            UWORD8 *pu1_pic_no_loop_filter_flag;
            WORD32 numbytes_row;
            UWORD32 u4_mask;

            pu1_pic_no_loop_filter_flag = ps_codec->s_parse.pu1_pic_no_loop_filter_flag;
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
  
        ps_codec->s_parse.ps_tu++;
        ps_codec->s_parse.s_cu.i4_tu_cnt++;
        ps_codec->s_parse.i4_pic_tu_idx++;

 }
 else
 {
        WORD32 cnt = 0;
        WORD32 i;
        WORD32 part_cnt;

        part_cnt = (part_mode == PART_NxN) ? 4 : 1;

 for(i = 0; i < part_cnt; i++)
 {
            TRACE_CABAC_CTXT("prev_intra_pred_luma_flag", ps_cabac->u4_range, IHEVC_CAB_INTRA_LUMA_PRED_FLAG);
            value = ihevcd_cabac_decode_bin(ps_cabac,
                                            ps_bitstrm,
                                            IHEVC_CAB_INTRA_LUMA_PRED_FLAG);

            ps_codec->s_parse.s_cu.ai4_prev_intra_luma_pred_flag[i] =
                            value;
            AEV_TRACE("prev_intra_pred_luma_flag", value, ps_cabac->u4_range);
 }

 for(i = 0; i < part_cnt; i++)
 {
 if(ps_codec->s_parse.s_cu.ai4_prev_intra_luma_pred_flag[cnt])
 {
                value = ihevcd_cabac_decode_bypass_bins_tunary(ps_cabac, ps_bitstrm, 2);
                AEV_TRACE("mpm_idx", value, ps_cabac->u4_range);
                ps_codec->s_parse.s_cu.ai4_mpm_idx[cnt] = value;
 }
 else
 {
                value = ihevcd_cabac_decode_bypass_bins(ps_cabac, ps_bitstrm, 5);
                AEV_TRACE("rem_intra_luma_pred_mode", value,
                          ps_cabac->u4_range);
                ps_codec->s_parse.s_cu.ai4_rem_intra_luma_pred_mode[cnt] =
                                value;
 }
            cnt++;
 }
        TRACE_CABAC_CTXT("intra_chroma_pred_mode", ps_cabac->u4_range, IHEVC_CAB_CHROMA_PRED_MODE);
        value = ihevcd_cabac_decode_bin(ps_cabac,
                                        ps_bitstrm,
                                        IHEVC_CAB_CHROMA_PRED_MODE);
        ps_codec->s_parse.s_cu.i4_intra_chroma_pred_mode_idx = 4;
 if(value)
 {
            ps_codec->s_parse.s_cu.i4_intra_chroma_pred_mode_idx =
                            ihevcd_cabac_decode_bypass_bins(ps_cabac,
                                                            ps_bitstrm, 2);
 }
        AEV_TRACE("intra_chroma_pred_mode",
                  ps_codec->s_parse.s_cu.i4_intra_chroma_pred_mode_idx,
                  ps_cabac->u4_range);


        ihevcd_intra_pred_mode_prediction(ps_codec, log2_cb_size, x0, y0);
 }
    STATS_UPDATE_PU_SIZE(ps_pu);
  
    ps_codec->s_parse.ps_pu++;
    ps_codec->s_parse.i4_pic_pu_idx++;

 return ret;
}
