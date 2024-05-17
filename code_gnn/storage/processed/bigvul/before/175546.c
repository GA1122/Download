WORD32 ihevcd_nctb_cnt(codec_t *ps_codec, sps_t *ps_sps)
{
    WORD32 nctb = 1;
    UNUSED(ps_codec);
  
 if(ps_sps->i1_log2_ctb_size < 5)
        nctb = 1;

 return nctb;
}
