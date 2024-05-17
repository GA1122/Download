UWORD32 ihevcd_cabac_decode_bypass_bins_trunc_rice(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm,
                                                   WORD32 c_rice_param,
                                                   WORD32 c_rice_max)
{
    UWORD32 u4_sym;
    WORD32 bin;
    WORD32 c_max;
    UWORD32 u4_suffix;
  
    ASSERT((c_rice_param >= 0));


  
    c_max = c_rice_max >> c_rice_param;
    u4_sym = 0;
 do
 {
        IHEVCD_CABAC_DECODE_BYPASS_BIN(bin, ps_cabac, ps_bitstrm);
        u4_sym++;

 }while(((WORD32)u4_sym < c_max) && bin);
    u4_sym = u4_sym - 1 + bin;

  
 if(c_rice_param)
 {
        IHEVCD_CABAC_DECODE_BYPASS_BINS(u4_suffix, ps_cabac, ps_bitstrm, c_rice_param);

        u4_sym = (u4_sym << c_rice_param) | u4_suffix;
 }
 return (u4_sym);
}
