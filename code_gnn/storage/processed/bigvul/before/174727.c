UWORD32 ih264d_uev(UWORD32 *pu4_bitstrm_ofst, UWORD32 *pu4_bitstrm_buf)
{
    UWORD32 u4_bitstream_offset = *pu4_bitstrm_ofst;
    UWORD32 u4_word, u4_ldz;

  
  
  
    NEXTBITS_32(u4_word, u4_bitstream_offset, pu4_bitstrm_buf);
    u4_ldz = CLZ(u4_word);
  
    u4_bitstream_offset += (u4_ldz + 1);
  
    u4_word = 0;
 if(u4_ldz)
        GETBITS(u4_word, u4_bitstream_offset, pu4_bitstrm_buf, u4_ldz);
 *pu4_bitstrm_ofst = u4_bitstream_offset;
 return ((1 << u4_ldz) + u4_word - 1);
}
