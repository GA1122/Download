UWORD32 ihevcd_cabac_decode_terminate(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm)
{
    UWORD32 u4_range = ps_cabac->u4_range;
    UWORD32 u4_ofst = ps_cabac->u4_ofst;
    UWORD32 u4_bin;
#if FULLRANGE
    WORD32 clz;
#endif
  
    ASSERT(u4_range >= 256);
#if FULLRANGE
    clz = CLZ(u4_range);
    clz -= (32 - RANGE_NUMBITS);
    u4_range -= 2 << (RANGE_SHIFT - clz);
#else
    u4_range -= 2;
#endif

 if(u4_ofst >= u4_range)
 {
        u4_bin = 1;

#if FULLRANGE
  
 {
            WORD32 clz;
            WORD32 numbits;
            clz = CLZ(ps_cabac->u4_range);

            numbits = (32 - clz);
            numbits -= 9;

            ihevcd_bits_seek(ps_bitstrm, -numbits);
 }
#endif

 }
 else
 {
        u4_bin = 0;
 }
 if(0 == u4_bin)
 {
        UWORD32 u4_bits;
        WORD32 numbits;
        numbits = CLZ(u4_range);
        numbits -= (32 - RANGE_NUMBITS);
#if !FULLRANGE
        numbits -= RANGE_SHIFT;
#endif
  
 if(numbits)
 {
            BITS_GET(u4_bits, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                     ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, numbits);

            u4_ofst <<= numbits;
            u4_ofst |= u4_bits;
            u4_range <<= numbits;
 }
 }
  
    ps_cabac->u4_range = u4_range;
    ps_cabac->u4_ofst = u4_ofst;
    DEBUG_RANGE_OFST("term", ps_cabac->u4_range, ps_cabac->u4_ofst);

 return (u4_bin);
}
