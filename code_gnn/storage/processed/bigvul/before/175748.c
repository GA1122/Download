IHEVCD_ERROR_T  ihevcd_parse_sao(codec_t *ps_codec)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 sps_t *ps_sps;
 sao_t *ps_sao;
    WORD32 rx;
    WORD32 ry;
    WORD32 value;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
    WORD32 sao_merge_left_flag;
    WORD32 sao_merge_up_flag;
 slice_header_t *ps_slice_hdr;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;
    WORD32 ctxt_idx;

    ps_slice_hdr = ps_codec->s_parse.ps_slice_hdr_base;
    ps_slice_hdr += (ps_codec->s_parse.i4_cur_slice_idx & (MAX_SLICE_HDR_CNT - 1));

    ps_sps = (ps_codec->s_parse.ps_sps);
    rx = ps_codec->s_parse.i4_ctb_x;
    ry = ps_codec->s_parse.i4_ctb_y;

    ps_sao = ps_codec->s_parse.ps_pic_sao + rx + ry * ps_sps->i2_pic_wd_in_ctb;

  
    ps_sao->b3_y_type_idx = 0;
    ps_sao->b3_cb_type_idx = 0;
    ps_sao->b3_cr_type_idx = 0;

    UNUSED(value);
    ctxt_idx = IHEVC_CAB_SAO_MERGE;
    sao_merge_left_flag = 0;
    sao_merge_up_flag = 0;
 if(rx > 0)
 {
  
 if(((0 != ps_codec->s_parse.i4_ctb_slice_x) || (0 != ps_codec->s_parse.i4_ctb_slice_y)) &&
 (0 != ps_codec->s_parse.i4_ctb_tile_x))
 {

            TRACE_CABAC_CTXT("sao_merge_flag", ps_cabac->u4_range, ctxt_idx);
            sao_merge_left_flag = ihevcd_cabac_decode_bin(ps_cabac,
                                                          ps_bitstrm,
                                                          ctxt_idx);
            AEV_TRACE("sao_merge_flag", sao_merge_left_flag, ps_cabac->u4_range);
 }

 }
 if(ry > 0 && !sao_merge_left_flag)
 {
 if((ps_codec->s_parse.i4_ctb_slice_y > 0) && (ps_codec->s_parse.i4_ctb_tile_y > 0))
 {
            TRACE_CABAC_CTXT("sao_merge_flag", ps_cabac->u4_range, ctxt_idx);
            sao_merge_up_flag = ihevcd_cabac_decode_bin(ps_cabac,
                                                        ps_bitstrm,
                                                        ctxt_idx);
            AEV_TRACE("sao_merge_flag", sao_merge_up_flag, ps_cabac->u4_range);
 }
 }
    ctxt_idx = IHEVC_CAB_SAO_TYPE;

 if(sao_merge_left_flag)
 {
 *ps_sao = *(ps_sao - 1);
 }
 else if(sao_merge_up_flag)
 {
 *ps_sao = *(ps_sao - ps_sps->i2_pic_wd_in_ctb);
 }
 else  
 {
        WORD32 c_idx;
        WORD32 sao_type_idx = 0;
 for(c_idx = 0; c_idx < 3; c_idx++)
 {
 if((ps_slice_hdr->i1_slice_sao_luma_flag && c_idx == 0) || (ps_slice_hdr->i1_slice_sao_chroma_flag && c_idx > 0))
 {


  

 if(c_idx == 0)
 {
                    sao_type_idx = 0;
                    TRACE_CABAC_CTXT("sao_type_idx", ps_cabac->u4_range, ctxt_idx);
                    sao_type_idx = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);

 if(sao_type_idx)
 {
                        sao_type_idx += ihevcd_cabac_decode_bypass_bin(ps_cabac, ps_bitstrm);
 }
                    AEV_TRACE("sao_type_idx", sao_type_idx,  ps_cabac->u4_range);

                    ps_sao->b3_y_type_idx = sao_type_idx;
 }
 if(c_idx == 1)
 {
                    sao_type_idx = 0;
                    TRACE_CABAC_CTXT("sao_type_idx", ps_cabac->u4_range, ctxt_idx);
                    sao_type_idx = ihevcd_cabac_decode_bin(ps_cabac, ps_bitstrm, ctxt_idx);
 if(sao_type_idx)
 {
                        sao_type_idx += ihevcd_cabac_decode_bypass_bin(ps_cabac, ps_bitstrm);
 }

                    AEV_TRACE("sao_type_idx", sao_type_idx,  ps_cabac->u4_range);

                    ps_sao->b3_cb_type_idx = sao_type_idx;
                    ps_sao->b3_cr_type_idx = sao_type_idx;
 }

 if(sao_type_idx != 0)
 {
                    WORD32 i;
                    WORD32 sao_offset[4];
                    WORD32 sao_band_position = 0;
                    WORD32 c_max = (1 << (MIN(BIT_DEPTH, 10) - 5)) - 1;
 for(i = 0; i < 4; i++)
 {
                        sao_offset[i] = ihevcd_cabac_decode_bypass_bins_tunary(ps_cabac, ps_bitstrm, c_max);
                        AEV_TRACE("sao_offset_abs", sao_offset[i], ps_cabac->u4_range);

 if((2 == sao_type_idx) && (i > 1))
 {
                            sao_offset[i] = -sao_offset[i];
 }
 }

 if(sao_type_idx == 1)
 {
 for(i = 0; i < 4; i++)
 {
 if(sao_offset[i] != 0)
 {
                                value = ihevcd_cabac_decode_bypass_bin(ps_cabac, ps_bitstrm);
                                AEV_TRACE("sao_offset_sign", value, ps_cabac->u4_range);

 if(value)
 {
                                    sao_offset[i] = -sao_offset[i];
 }
 }
 }
                        value = ihevcd_cabac_decode_bypass_bins(ps_cabac, ps_bitstrm, 5);
                        AEV_TRACE("sao_band_position", value, ps_cabac->u4_range);

                        sao_band_position = value;
 }
 else
 {
 if(c_idx == 0)
 {
                            value = ihevcd_cabac_decode_bypass_bins(ps_cabac, ps_bitstrm, 2);
                            AEV_TRACE("sao_eo_class", value, ps_cabac->u4_range);

                            ps_sao->b3_y_type_idx += value;
 }

 if(c_idx == 1)
 {
                            value = ihevcd_cabac_decode_bypass_bins(ps_cabac, ps_bitstrm, 2);
                            AEV_TRACE("sao_eo_class", value, ps_cabac->u4_range);

                            ps_sao->b3_cb_type_idx += value;
                            ps_sao->b3_cr_type_idx += value;
 }
 }

 if(0 == c_idx)
 {
                        ps_sao->b4_y_offset_1 = sao_offset[0];
                        ps_sao->b4_y_offset_2 = sao_offset[1];
                        ps_sao->b4_y_offset_3 = sao_offset[2];
                        ps_sao->b4_y_offset_4 = sao_offset[3];

                        ps_sao->b5_y_band_pos = sao_band_position;
 }
 else if(1 == c_idx)
 {
                        ps_sao->b4_cb_offset_1 = sao_offset[0];
                        ps_sao->b4_cb_offset_2 = sao_offset[1];
                        ps_sao->b4_cb_offset_3 = sao_offset[2];
                        ps_sao->b4_cb_offset_4 = sao_offset[3];

                        ps_sao->b5_cb_band_pos = sao_band_position;
 }
 else  
 {
                        ps_sao->b4_cr_offset_1 = sao_offset[0];
                        ps_sao->b4_cr_offset_2 = sao_offset[1];
                        ps_sao->b4_cr_offset_3 = sao_offset[2];
                        ps_sao->b4_cr_offset_4 = sao_offset[3];

                        ps_sao->b5_cr_band_pos = sao_band_position;
 }
 }
 }
 }
 }

 return ret;
}
