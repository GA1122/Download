IHEVCD_ERROR_T ihevcd_cabac_reset(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm)
{
  
    ASSERT(ps_cabac != NULL);
    ASSERT(ps_bitstrm != NULL);

  
  
#if FULLRANGE
    ps_cabac->u4_range = (UWORD32)510 << RANGE_SHIFT;
    BITS_GET(ps_cabac->u4_ofst, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                    ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, (9 + RANGE_SHIFT));

#else
    ps_cabac->u4_range = (UWORD32)510;
    BITS_GET(ps_cabac->u4_ofst, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                    ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, 9);

#endif

 return ((IHEVCD_ERROR_T)IHEVCD_SUCCESS);
}
