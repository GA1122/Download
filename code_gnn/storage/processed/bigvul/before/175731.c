UWORD32 ihevcd_cabac_decode_bin(cab_ctxt_t *ps_cabac,
 bitstrm_t *ps_bitstrm,
                                WORD32 ctxt_index

 )
{
    UWORD32 u4_range = ps_cabac->u4_range;
    UWORD32 u4_ofst = ps_cabac->u4_ofst;
    UWORD32 u4_rlps;
    UWORD32 u4_bin;
    UWORD8 *pu1_ctxt_model = &ps_cabac->au1_ctxt_models[ctxt_index];
    WORD32 state_mps = *pu1_ctxt_model;
#if FULLRANGE
    WORD32 clz;
#endif
    UWORD32 u4_qnt_range;

  
    ASSERT(u4_range >= 256);
    ASSERT((ctxt_index >= 0) && (ctxt_index < IHEVC_CAB_CTXT_END));
    ASSERT(state_mps < 128);
#if FULLRANGE
    clz = CLZ(u4_range);
    clz -= (32 - RANGE_NUMBITS);
    u4_qnt_range = u4_range << clz;
    u4_qnt_range = (u4_qnt_range >> (RANGE_SHIFT + 6)) & 0x3;
#else
    u4_qnt_range = (u4_range >> 6) & 0x3;
#endif
  
    u4_rlps = gau1_ihevc_cabac_rlps[state_mps >> 1][u4_qnt_range];
#if FULLRANGE
    u4_rlps = u4_rlps << (RANGE_SHIFT - clz);
#endif
    u4_range -= u4_rlps;

    u4_bin = state_mps & 1;

 if(u4_ofst >= u4_range)
 {
        u4_bin = 1 - u4_bin;
        u4_ofst -= u4_range;
        u4_range = u4_rlps;
 }

 *pu1_ctxt_model = gau1_ihevc_next_state[(state_mps << 1) | u4_bin];

  
  
  
 if(u4_range < (1 << 8))
 {
        UWORD32 u4_bits;
        WORD32 numbits;
        numbits = CLZ(u4_range);
        numbits -= (32 - RANGE_NUMBITS);
#if !FULLRANGE
        numbits -= RANGE_SHIFT;
#endif
        BITS_GET(u4_bits, ps_bitstrm->pu4_buf, ps_bitstrm->u4_bit_ofst,
                 ps_bitstrm->u4_cur_word, ps_bitstrm->u4_nxt_word, numbits);

        u4_ofst <<= numbits;
        u4_ofst |= u4_bits;
        u4_range <<= numbits;

 }
  
    ps_cabac->u4_range = u4_range;
    ps_cabac->u4_ofst = u4_ofst;
    DEBUG_RANGE_OFST("bin", ps_cabac->u4_range, ps_cabac->u4_ofst);

 return (u4_bin);


}
