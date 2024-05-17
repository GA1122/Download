IHEVCD_ERROR_T  ihevcd_parse_pcm_sample(codec_t *ps_codec,
                                        WORD32 x0,
                                        WORD32 y0,
                                        WORD32 log2_cb_size)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 cab_ctxt_t *ps_cabac = &ps_codec->s_parse.s_cabac;
 sps_t *ps_sps;

    WORD32 value;
    WORD32 i;

    WORD32 num_bits;
    UWORD32 u4_sig_coeff_map;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
 tu_t *ps_tu = ps_codec->s_parse.ps_tu;
 tu_sblk_coeff_data_t *ps_tu_sblk_coeff_data;
    UWORD8 *pu1_coeff_data;
    ps_sps = ps_codec->s_parse.ps_sps;

    UNUSED(value);
    UNUSED(ps_tu);
    UNUSED(ps_cabac);
    UNUSED(x0);
    UNUSED(y0);

 {
        WORD8 *pi1_scan_idx;
        WORD8 *pi1_buf = (WORD8 *)ps_codec->s_parse.pv_tu_coeff_data;
        WORD8 *pi1_num_coded_subblks;

  
        pi1_num_coded_subblks = pi1_buf++;

  
  
 *pi1_num_coded_subblks = 1;

  
        pi1_scan_idx = pi1_buf++;
 *pi1_scan_idx = (0 << 1) | 1;

  
        ps_codec->s_parse.pv_tu_coeff_data = pi1_buf;

 }

    u4_sig_coeff_map = 0xFFFFFFFF;
    ps_tu_sblk_coeff_data = (tu_sblk_coeff_data_t *)ps_codec->s_parse.pv_tu_coeff_data;
    ps_tu_sblk_coeff_data->u2_sig_coeff_map = u4_sig_coeff_map;
    ps_tu_sblk_coeff_data->u2_subblk_pos = 0;

    pu1_coeff_data = (UWORD8 *)&ps_tu_sblk_coeff_data->ai2_level[0];

    num_bits = ps_sps->i1_pcm_sample_bit_depth_luma;

 for(i = 0; i < 1 << (log2_cb_size << 1); i++)
 {
        TRACE_CABAC_CTXT("pcm_sample_luma", ps_cabac->u4_range, 0);
        BITS_PARSE("pcm_sample_luma", value, ps_bitstrm, num_bits);

 *pu1_coeff_data++ = value << (BIT_DEPTH_LUMA - num_bits);
 }

    num_bits = ps_sps->i1_pcm_sample_bit_depth_chroma;

 for(i = 0; i < (1 << (log2_cb_size << 1)) >> 1; i++)
 {
        TRACE_CABAC_CTXT("pcm_sample_chroma", ps_cabac->u4_range, 0);
        BITS_PARSE("pcm_sample_chroma", value, ps_bitstrm, num_bits);

 *pu1_coeff_data++ = value << (BIT_DEPTH_CHROMA - num_bits);
 }

    ps_codec->s_parse.pv_tu_coeff_data = pu1_coeff_data;

 return ret;
}
