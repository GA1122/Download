void ih264d_parse_pmb_ref_index_cavlc_range1(UWORD32 u4_num_part,  
 dec_bit_stream_t *ps_bitstrm,  
                                             WORD8 *pi1_ref_idx,  
                                             UWORD32 u4_num_ref_idx_active_minus1  
 )
{
    UWORD32 u4_i;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstream_off = &ps_bitstrm->u4_ofst;
    UNUSED(u4_num_ref_idx_active_minus1);
 for(u4_i = 0; u4_i < u4_num_part; u4_i++)
 {
        UWORD32 u4_ref_idx;
        u4_ref_idx = ih264d_tev_range1(pu4_bitstream_off, pu4_bitstrm_buf);

  
        pi1_ref_idx[u4_i] = (WORD8)u4_ref_idx;
 }
}
