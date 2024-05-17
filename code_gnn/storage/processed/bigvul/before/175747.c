IHEVCD_ERROR_T  ihevcd_parse_pu_mvp(codec_t *ps_codec, pu_t *ps_pu)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 value;
 slice_header_t *ps_slice_hdr;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;
    WORD32 inter_pred_idc;

    ps_slice_hdr = ps_codec->s_parse.ps_slice_hdr;

 if(ps_slice_hdr->i1_slice_type == BSLICE)
 {
        WORD32 pu_w_plus_pu_h;
        WORD32 ctxt_idx;
  
        pu_w_plus_pu_h = ((ps_pu->b4_wd + 1) << 2) + ((ps_pu->b4_ht + 1) << 2);
 if(12 == pu_w_plus_pu_h)
 {
            ctxt_idx = IHEVC_CAB_INTER_PRED_IDC + 4;
            TRACE_CABAC_CTXT("inter_pred_idc", ps_cabac->u4_range, ctxt_idx);
            inter_pred_idc = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm,
                                                     ctxt_idx);
 }
 else
 {
  
            WORD32 is_bipred;

            ctxt_idx = IHEVC_CAB_INTER_PRED_IDC + ps_codec->s_parse.i4_ct_depth;
            TRACE_CABAC_CTXT("inter_pred_idc", ps_cabac->u4_range, ctxt_idx);
            is_bipred = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
            inter_pred_idc = PRED_BI;
 if(!is_bipred)
 {
                ctxt_idx = IHEVC_CAB_INTER_PRED_IDC + 4;
                inter_pred_idc = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm,
                                                         ctxt_idx);
 }
 }

        AEV_TRACE("inter_pred_idc", inter_pred_idc, ps_cabac->u4_range);
 }
 else
        inter_pred_idc = PRED_L0;
    ps_pu->mv.i1_l0_ref_idx = 0;
    ps_pu->mv.i1_l1_ref_idx = 0;
  
 if(inter_pred_idc != PRED_L1)
 {
        WORD32 active_refs = ps_slice_hdr->i1_num_ref_idx_l0_active;
        WORD32 ref_idx = 0;
        WORD32 ctxt_idx;

 if(active_refs > 1)
 {
            ctxt_idx = IHEVC_CAB_INTER_REF_IDX;
  
            TRACE_CABAC_CTXT("ref_idx", ps_cabac->u4_range, ctxt_idx);
            ref_idx = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);

 if((active_refs > 2) && ref_idx)
 {
                WORD32 value;
  
                ctxt_idx++;
                value = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
                ref_idx += value;
 if((active_refs > 3) && value)
 {
  
                    ref_idx = ihevcd_cabac_decode_bypass_bins_tunary(ps_cabac,
                                                                     ps_bitstrm,
 (active_refs - 3)
 );
                    ref_idx += 2;
 }
 }
            AEV_TRACE("ref_idx", ref_idx, ps_cabac->u4_range);
 }

        ref_idx = CLIP3(ref_idx, 0, MAX_DPB_SIZE - 1);
        ps_pu->mv.i1_l0_ref_idx = ref_idx;

        ihevcd_parse_mvd(ps_codec, &ps_pu->mv.s_l0_mv);

        ctxt_idx = IHEVC_CAB_MVP_L0L1;
        value = ihevcd_cabac_decode_bin(ps_cabac,
                                        ps_bitstrm,
                                        ctxt_idx);

        AEV_TRACE("mvp_l0/l1_flag", value, ps_cabac->u4_range);

        ps_pu->b1_l0_mvp_idx = value;

 }
  
 if(inter_pred_idc != PRED_L0)
 {
        WORD32 active_refs = ps_slice_hdr->i1_num_ref_idx_l1_active;
        WORD32 ref_idx = 0;
        WORD32 ctxt_idx;

 if(active_refs > 1)
 {

            ctxt_idx = IHEVC_CAB_INTER_REF_IDX;
            TRACE_CABAC_CTXT("ref_idx", ps_cabac->u4_range, ctxt_idx);
  
            ref_idx = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);

 if((active_refs > 2) && ref_idx)
 {
                WORD32 value;
  
                ctxt_idx++;
                value = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
                ref_idx += value;
 if((active_refs > 3) && value)
 {
  
                    ref_idx = ihevcd_cabac_decode_bypass_bins_tunary(ps_cabac,
                                                                     ps_bitstrm,
 (active_refs - 3)
 );
                    ref_idx += 2;
 }
 }

            AEV_TRACE("ref_idx", ref_idx, ps_cabac->u4_range);
 }

        ref_idx = CLIP3(ref_idx, 0, MAX_DPB_SIZE - 1);
        ps_pu->mv.i1_l1_ref_idx = ref_idx;

 if(ps_slice_hdr->i1_mvd_l1_zero_flag && inter_pred_idc == PRED_BI)
 {
            ps_pu->mv.s_l1_mv.i2_mvx = 0;
            ps_pu->mv.s_l1_mv.i2_mvy = 0;
 }
 else
 {
            ihevcd_parse_mvd(ps_codec, &ps_pu->mv.s_l1_mv);
 }

        ctxt_idx = IHEVC_CAB_MVP_L0L1;
        value = ihevcd_cabac_decode_bin(ps_cabac,
                                        ps_bitstrm,
                                        ctxt_idx);

        AEV_TRACE("mvp_l0/l1_flag", value, ps_cabac->u4_range);
        ps_pu->b1_l1_mvp_idx = value;

 }

    ps_pu->b2_pred_mode = inter_pred_idc;
 return ret;
}
