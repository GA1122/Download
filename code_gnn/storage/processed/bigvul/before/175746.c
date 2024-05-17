IHEVCD_ERROR_T  ihevcd_parse_prediction_unit(codec_t *ps_codec,
                                             WORD32 x0,
                                             WORD32 y0,
                                             WORD32 wd,
                                             WORD32 ht)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 slice_header_t *ps_slice_hdr;
 sps_t *ps_sps;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
    WORD32 ctb_x_base;
    WORD32 ctb_y_base;

 pu_t *ps_pu = ps_codec->s_parse.ps_pu;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;

    ps_slice_hdr = ps_codec->s_parse.ps_slice_hdr;

  
    memset(ps_pu, 0, sizeof(pu_t));

    ps_sps = ps_codec->s_parse.ps_sps;
    ctb_x_base = ps_codec->s_parse.i4_ctb_x << ps_sps->i1_log2_ctb_size;
    ctb_y_base = ps_codec->s_parse.i4_ctb_y << ps_sps->i1_log2_ctb_size;

    ps_pu->b4_pos_x = (x0 - ctb_x_base) >> 2;
    ps_pu->b4_pos_y = (y0 - ctb_y_base) >> 2;
    ps_pu->b4_wd = (wd >> 2) - 1;
    ps_pu->b4_ht = (ht >> 2) - 1;

    ps_pu->b1_intra_flag = 0;
    ps_pu->b3_part_mode = ps_codec->s_parse.s_cu.i4_part_mode;

 if(PRED_MODE_SKIP == ps_codec->s_parse.s_cu.i4_pred_mode)
 {
        WORD32 merge_idx = 0;
 if(ps_slice_hdr->i1_max_num_merge_cand > 1)
 {
            WORD32 ctxt_idx = IHEVC_CAB_MERGE_IDX_EXT;
            WORD32 bin;

            TRACE_CABAC_CTXT("merge_idx", ps_cabac->u4_range, ctxt_idx);
            bin = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
 if(bin)
 {
 if(ps_slice_hdr->i1_max_num_merge_cand > 2)
 {
                    merge_idx = ihevcd_cabac_decode_bypass_bins_tunary(
                                    ps_cabac, ps_bitstrm,
 (ps_slice_hdr->i1_max_num_merge_cand - 2));
 }
                merge_idx++;
 }
            AEV_TRACE("merge_idx", merge_idx, ps_cabac->u4_range);
 }
        ps_pu->b1_merge_flag = 1;
        ps_pu->b3_merge_idx = merge_idx;

 }
 else
 {
  
        WORD32 merge_flag;
        WORD32 ctxt_idx = IHEVC_CAB_MERGE_FLAG_EXT;
        TRACE_CABAC_CTXT("merge_flag", ps_cabac->u4_range, ctxt_idx);
        merge_flag = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
        AEV_TRACE("merge_flag", merge_flag, ps_cabac->u4_range);

        ps_pu->b1_merge_flag = merge_flag;

 if(merge_flag)
 {
            WORD32 merge_idx = 0;
 if(ps_slice_hdr->i1_max_num_merge_cand > 1)
 {
                WORD32 ctxt_idx = IHEVC_CAB_MERGE_IDX_EXT;
                WORD32 bin;
                TRACE_CABAC_CTXT("merge_idx", ps_cabac->u4_range, ctxt_idx);
                bin = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
 if(bin)
 {
 if(ps_slice_hdr->i1_max_num_merge_cand > 2)
 {
                        merge_idx = ihevcd_cabac_decode_bypass_bins_tunary(
                                        ps_cabac, ps_bitstrm,
 (ps_slice_hdr->i1_max_num_merge_cand - 2));
 }
                    merge_idx++;
 }
                AEV_TRACE("merge_idx", merge_idx, ps_cabac->u4_range);
 }

            ps_pu->b3_merge_idx = merge_idx;
 }
 else
 {
            ihevcd_parse_pu_mvp(ps_codec, ps_pu);
 }

 }
    STATS_UPDATE_PU_SIZE(ps_pu);
  
    ps_codec->s_parse.ps_pu++;
    ps_codec->s_parse.i4_pic_pu_idx++;
 return ret;
}
