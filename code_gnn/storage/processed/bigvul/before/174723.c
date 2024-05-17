WORD32 ih264d_parse_pmb_ref_index_cavlc(UWORD32 u4_num_part,  
 dec_bit_stream_t *ps_bitstrm,  
                                      WORD8 *pi1_ref_idx,  
                                      UWORD32 u4_num_ref_idx_active_minus1  
 )
{
    UWORD32 u4_i;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 *pu4_bitstream_off = &ps_bitstrm->u4_ofst;

 for(u4_i = 0; u4_i < u4_num_part; u4_i++)
 {
        UWORD32 u4_ref_idx;
        UWORD32 u4_bitstream_offset = *pu4_bitstream_off;
        UWORD32 u4_word, u4_ldz;

  
  
  
        NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
        u4_ldz = CLZ(u4_word);
  
        u4_bitstream_offset += (u4_ldz + 1);
  
        u4_word = 0;
 if(u4_ldz)
            GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf, u4_ldz);
 *pu4_bitstream_off = u4_bitstream_offset;
        u4_ref_idx = ((1 << u4_ldz) + u4_word - 1);

 if(u4_ref_idx > u4_num_ref_idx_active_minus1)
 return ERROR_REF_IDX;

  
        pi1_ref_idx[u4_i] = (WORD8)u4_ref_idx;
 }
 return OK;
}
