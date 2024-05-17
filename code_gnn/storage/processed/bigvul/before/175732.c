UWORD32 ihevcd_cabac_decode_bins_tunary(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm,
                                        WORD32 c_max,
                                        WORD32 ctxt_index,
                                        WORD32 ctxt_shift,
                                        WORD32 ctxt_inc_max)
{
    UWORD32 u4_sym;
    WORD32 bin;

  
    ASSERT(c_max > 0);
    ASSERT((ctxt_index >= 0) && (ctxt_index < IHEVC_CAB_CTXT_END));
    ASSERT((ctxt_index + (c_max >> ctxt_shift)) < IHEVC_CAB_CTXT_END);

    u4_sym = 0;
 do
 {
        WORD32 bin_index;
        bin_index = ctxt_index + MIN((u4_sym >> ctxt_shift), (UWORD32)ctxt_inc_max);
        IHEVCD_CABAC_DECODE_BIN(bin, ps_cabac, ps_bitstrm,  bin_index);
        u4_sym++;
 }while(((WORD32)u4_sym < c_max) && bin);

    u4_sym = u4_sym - 1 + bin;

 return (u4_sym);
}
