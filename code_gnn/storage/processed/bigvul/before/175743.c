IHEVCD_ERROR_T ihevcd_parse_mvd(codec_t *ps_codec, mv_t *ps_mv)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 value;
    WORD32 abs_mvd;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
    WORD32 abs_mvd_greater0_flag[2];
    WORD32 abs_mvd_greater1_flag[2];
    WORD32 ctxt_idx;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;


    ctxt_idx  = IHEVC_CAB_MVD_GRT0;
  
    TRACE_CABAC_CTXT("abs_mvd_greater0_flag[0]", ps_cabac->u4_range, ctxt_idx);
    abs_mvd_greater0_flag[0] = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
    AEV_TRACE("abs_mvd_greater0_flag[0]", abs_mvd_greater0_flag[0], ps_cabac->u4_range);

  
    TRACE_CABAC_CTXT("abs_mvd_greater0_flag[1]", ps_cabac->u4_range, ctxt_idx);
    abs_mvd_greater0_flag[1] = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
    AEV_TRACE("abs_mvd_greater0_flag[1]", abs_mvd_greater0_flag[1], ps_cabac->u4_range);

    ctxt_idx  = IHEVC_CAB_MVD_GRT1;
    abs_mvd_greater1_flag[0] = 0;
    abs_mvd_greater1_flag[1] = 0;

 if(abs_mvd_greater0_flag[0])
 {
        TRACE_CABAC_CTXT("abs_mvd_greater1_flag[0]", ps_cabac->u4_range, ctxt_idx);
        abs_mvd_greater1_flag[0] = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
        AEV_TRACE("abs_mvd_greater1_flag[0]", abs_mvd_greater1_flag[0], ps_cabac->u4_range);
 }
 if(abs_mvd_greater0_flag[1])
 {
        TRACE_CABAC_CTXT("abs_mvd_greater1_flag[1]", ps_cabac->u4_range, ctxt_idx);
        abs_mvd_greater1_flag[1] = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
        AEV_TRACE("abs_mvd_greater1_flag[1]", abs_mvd_greater1_flag[1], ps_cabac->u4_range);
 }
    abs_mvd = 0;
 if(abs_mvd_greater0_flag[0])
 {
        abs_mvd = 1;
 if(abs_mvd_greater1_flag[0])
 {
            value = ihevcd_cabac_decode_bypass_bins_egk(ps_cabac, ps_bitstrm, 1);
            AEV_TRACE("abs_mvd_minus2[0]", value, ps_cabac->u4_range);
            abs_mvd = value + 2;
 }
        value = ihevcd_cabac_decode_bypass_bin(ps_cabac, ps_bitstrm);
        AEV_TRACE("mvd_sign_flag[0]", value, ps_cabac->u4_range);
 if(value)
 {
            abs_mvd = -abs_mvd;
 }

 }
    ps_mv->i2_mvx = abs_mvd;
    abs_mvd = 0;
 if(abs_mvd_greater0_flag[1])
 {
        abs_mvd = 1;
 if(abs_mvd_greater1_flag[1])
 {
            value = ihevcd_cabac_decode_bypass_bins_egk(ps_cabac, ps_bitstrm, 1);
            AEV_TRACE("abs_mvd_minus2[1]", value, ps_cabac->u4_range);
            abs_mvd = value + 2;

 }
        value = ihevcd_cabac_decode_bypass_bin(ps_cabac, ps_bitstrm);
        AEV_TRACE("mvd_sign_flag[1]", value, ps_cabac->u4_range);

 if(value)
 {
            abs_mvd = -abs_mvd;
 }
 }
    ps_mv->i2_mvy = abs_mvd;

 return ret;
}
