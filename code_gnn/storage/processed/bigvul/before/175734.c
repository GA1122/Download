UWORD32 ihevcd_cabac_decode_bypass_bins(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm,
                                        WORD32 numbins)
{
    UWORD32 u4_bins;


    UWORD32 u4_range = ps_cabac->u4_range;
    UWORD32 u4_ofst = ps_cabac->u4_ofst;
    UWORD32 u4_bits;
    ASSERT(u4_range >= 256);
    ASSERT(numbins > 0);

  
    ASSERT(numbins < 17);

    u4_bins = 0;

    BITS_GET(u4_bits, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                    ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, numbins);

 do
 {
        UWORD32 u4_bit;
        numbins--;
        u4_bit = (u4_bits >> numbins) & 1;
        u4_ofst <<= 1;
        u4_ofst |= u4_bit;

        u4_bins <<= 1;
 if(u4_ofst >= u4_range)
 {
            u4_bins += 1;
            u4_ofst -= u4_range;
 }
 }while(numbins);

  
    ps_cabac->u4_ofst = u4_ofst;
    DEBUG_RANGE_OFST("bypass", ps_cabac->u4_range, ps_cabac->u4_ofst);
 return (u4_bins);
}
