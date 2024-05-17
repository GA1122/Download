UWORD32 ihevcd_cabac_decode_bypass_bins_tunary(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm,
                                               WORD32 c_max)
{

    UWORD32 u4_sym;
    WORD32 bin;
    UWORD32 u4_ofst = ps_cabac->u4_ofst;
    UWORD32 u4_range = ps_cabac->u4_range;
    UWORD32 u4_bits;
  
    ASSERT(c_max > 0);
    ASSERT(u4_range >= 256);
    u4_sym = 0;
    BITS_NXT(u4_bits, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                    ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, (UWORD32)c_max);
    u4_bits <<= (32 - c_max);
 do
 {
        u4_ofst <<= 1;
        u4_ofst |= (u4_bits >> 31);
        u4_bits <<= 1;

        bin = 0;
 if(u4_ofst >= u4_range)
 {
            bin = 1;
            u4_ofst -= u4_range;
 }
        u4_sym++;
 }while(((WORD32)u4_sym < c_max) && bin);
    BITS_FLUSH(ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                    ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, u4_sym);

    u4_sym = u4_sym - 1 + bin;
  
    ps_cabac->u4_ofst = u4_ofst;

 return (u4_sym);
}
