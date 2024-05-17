UWORD32 ihevcd_cabac_decode_bypass_bin(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm)
{

    UWORD32 u4_bin;
    UWORD32 u4_range = ps_cabac->u4_range;
    UWORD32 u4_ofst = ps_cabac->u4_ofst;
    UWORD32 u4_bits;

  
    ASSERT(u4_range >= 256);

    BIT_GET(u4_bits, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
            ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word);

    u4_ofst <<= 1;
    u4_ofst |= u4_bits;

    u4_bin = 0;
 if(u4_ofst >= u4_range)
 {
        u4_bin = 1;
        u4_ofst -= u4_range;
 }

  
    ps_cabac->u4_ofst = u4_ofst;
    DEBUG_RANGE_OFST("bypass end", ps_cabac->u4_range, ps_cabac->u4_ofst);
 return (u4_bin);
}
