WORD32 ih264d_cavlc_4x4res_block_totalcoeff_1(UWORD32 u4_isdc,
                                           UWORD32 u4_total_coeff_trail_one,
 dec_bit_stream_t *ps_bitstrm)
{

    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UWORD32 u4_bitstream_offset = ps_bitstrm->u4_ofst;
    UWORD32 u4_trailing_ones = u4_total_coeff_trail_one & 0xFFFF;
    WORD32 i2_level;
    UWORD32 u4_tot_zero, u4_ldz, u4_scan_pos;

 tu_sblk4x4_coeff_data_t *ps_tu_4x4;
    WORD16 *pi2_coeff_data;
 dec_struct_t *ps_dec = (dec_struct_t *)ps_bitstrm->pv_codec_handle;

    ps_tu_4x4 = (tu_sblk4x4_coeff_data_t *)ps_dec->pv_parse_tu_coeff_data;
    ps_tu_4x4->u2_sig_coeff_map = 0;
    pi2_coeff_data = &ps_tu_4x4->ai2_level[0];


 if(u4_trailing_ones)
 {
        UWORD32 u4_sign;
  
  
  
        GETBIT(u4_sign, u4_bitstream_offset, pu4_bitstrm_buf);
        i2_level = u4_sign ? -1 : 1;
 }
 else
 {
  
  
  
        UWORD32 u4_lev_suffix, u4_lev_suffix_size;
        WORD32 u2_lev_code, u2_abs_value;
        UWORD32 u4_lev_prefix;
  
  
  
        FIND_ONE_IN_STREAM_32(u4_lev_prefix, u4_bitstream_offset,
                              pu4_bitstrm_buf);
        u2_lev_code = (2 + MIN(u4_lev_prefix, 15));

 if(14 == u4_lev_prefix)
            u4_lev_suffix_size = 4;
 else if(15 <= u4_lev_prefix)
 {
            u2_lev_code += 15;
            u4_lev_suffix_size = u4_lev_prefix - 3;
 }
 else
            u4_lev_suffix_size = 0;

 if(16 <= u4_lev_prefix)
 {
            u2_lev_code += ((1 << (u4_lev_prefix - 3)) - 4096);
 }
 if(u4_lev_suffix_size)
 {
            GETBITS(u4_lev_suffix, u4_bitstream_offset, pu4_bitstrm_buf,
                    u4_lev_suffix_size);
            u2_lev_code += u4_lev_suffix;
 }

        u2_abs_value = (u2_lev_code + 2) >> 1;
  
  
  
        i2_level = (u2_lev_code & 1) ? -u2_abs_value : u2_abs_value;

 }

  
  
  
    FIND_ONE_IN_STREAM_LEN(u4_ldz, u4_bitstream_offset, pu4_bitstrm_buf, 8);

 if(u4_ldz)
 {
        GETBIT(u4_tot_zero, u4_bitstream_offset, pu4_bitstrm_buf);
        u4_tot_zero = (u4_ldz << 1) - u4_tot_zero;
 }
 else
        u4_tot_zero = 0;

  
  
  
    u4_scan_pos = u4_tot_zero + u4_isdc;
 if(u4_scan_pos > 15)
 return -1;

    SET_BIT(ps_tu_4x4->u2_sig_coeff_map, u4_scan_pos);
 *pi2_coeff_data++ = i2_level;


 {
        WORD32 offset;
        offset = (UWORD8 *)pi2_coeff_data - (UWORD8 *)ps_tu_4x4;
        offset = ALIGN4(offset);
        ps_dec->pv_parse_tu_coeff_data = (void *)((UWORD8 *)ps_dec->pv_parse_tu_coeff_data + offset);
 }

    ps_bitstrm->u4_ofst = u4_bitstream_offset;
 return 0;
}
