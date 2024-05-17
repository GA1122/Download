 WORD32 ih264d_cavlc_4x4res_block_totalcoeff_2to10(UWORD32 u4_isdc,
                                                UWORD32 u4_total_coeff_trail_one,  
  dec_bit_stream_t *ps_bitstrm)
 {
     UWORD32 u4_total_zeroes;
     WORD32 i;
     UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
 
      UWORD32 u4_bitstream_offset = ps_bitstrm->u4_ofst;
      UWORD32 u4_trailing_ones = u4_total_coeff_trail_one & 0xFFFF;
      UWORD32 u4_total_coeff = u4_total_coeff_trail_one >> 16;
    WORD16 i2_level_arr[16];
//      
//      
//      
//     WORD16 ai2_level_arr[19];
//     WORD16 *i2_level_arr = &ai2_level_arr[3];
  
      tu_sblk4x4_coeff_data_t *ps_tu_4x4;
      WORD16 *pi2_coeff_data;
  dec_struct_t *ps_dec = (dec_struct_t *)ps_bitstrm->pv_codec_handle;
 
     ps_tu_4x4 = (tu_sblk4x4_coeff_data_t *)ps_dec->pv_parse_tu_coeff_data;
     ps_tu_4x4->u2_sig_coeff_map = 0;
     pi2_coeff_data = &ps_tu_4x4->ai2_level[0];
 
     i = u4_total_coeff - 1;
 
  if(u4_trailing_ones)
  {
   
   
   
   
         UWORD32 u4_signs, u4_cnt = u4_trailing_ones;
         WORD16 (*ppi2_trlone_lkup)[3] =
  (WORD16 (*)[3])gai2_ih264d_trailing_one_level;
         WORD16 *pi2_trlone_lkup;
 
         GETBITS(u4_signs, u4_bitstream_offset, pu4_bitstrm_buf, u4_cnt);
 
         pi2_trlone_lkup = ppi2_trlone_lkup[(1 << u4_cnt) - 2 + u4_signs];
 
  while(u4_cnt--)
             i2_level_arr[i--] = *pi2_trlone_lkup++;
  }
 
   
   
   
  if(i >= 0)
  {
   
   
   
   
         UWORD32 u4_lev_suffix, u4_suffix_len, u4_lev_suffix_size;
         WORD32 u2_lev_code, u2_abs_value;
         UWORD32 u4_lev_prefix;
 
   
   
   
         FIND_ONE_IN_STREAM_32(u4_lev_prefix, u4_bitstream_offset,
                               pu4_bitstrm_buf);
 
   
   
   
         u2_lev_code = MIN(15, u4_lev_prefix);
 
         u2_lev_code += (3 == u4_trailing_ones) ? 0 : 2;
 
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
   
   
   
         i2_level_arr[i--] = (u2_lev_code & 1) ? -u2_abs_value : u2_abs_value;
 
         u4_suffix_len = (u2_abs_value > 3) ? 2 : 1;
 
   
   
   
  while(i >= 0)
  {
 
   
   
   
             FIND_ONE_IN_STREAM_32(u4_lev_prefix, u4_bitstream_offset,
                                   pu4_bitstrm_buf);
 
             u4_lev_suffix_size =
  (15 <= u4_lev_prefix) ?
  (u4_lev_prefix - 3) : u4_suffix_len;
 
   
   
   
             GETBITS(u4_lev_suffix, u4_bitstream_offset, pu4_bitstrm_buf,
                     u4_lev_suffix_size);
             u2_lev_code = (MIN(15,u4_lev_prefix) << u4_suffix_len)
  + u4_lev_suffix;
 
  if(16 <= u4_lev_prefix)
  {
                 u2_lev_code += ((1 << (u4_lev_prefix - 3)) - 4096);
  }
             u2_abs_value = (u2_lev_code + 2) >> 1;
 
   
   
   
             i2_level_arr[i--] =
  (u2_lev_code & 1) ? -u2_abs_value : u2_abs_value;
 
   
   
   
             u4_suffix_len +=
  (u4_suffix_len < 6) ?
  (u2_abs_value
  > (3
  << (u4_suffix_len
  - 1))) :
  0;
  }
 
   
   
   
  }
 
   
   
   
  {
         UWORD32 u4_index;
  const UWORD8 (*ppu1_total_zero_lkup)[64] =
  (const UWORD8 (*)[64])gau1_ih264d_table_total_zero_2to10;
 
         NEXTBITS(u4_index, u4_bitstream_offset, pu4_bitstrm_buf, 6);
         u4_total_zeroes = ppu1_total_zero_lkup[u4_total_coeff - 2][u4_index];
 
         FLUSHBITS(u4_bitstream_offset, (u4_total_zeroes >> 4));
         u4_total_zeroes &= 0xf;
  }
 
   
   
   
  {
  const UWORD8 *pu1_table_runbefore;
         UWORD32 u4_run;
         WORD32 k;
         UWORD32 u4_scan_pos = u4_total_coeff + u4_total_zeroes - 1 + u4_isdc;
         WORD32 u4_zeroes_left = u4_total_zeroes;
         k = u4_total_coeff - 1;
 
   
   
   
  while((u4_zeroes_left > 6) && k)
  {
             UWORD32 u4_code;
 
             NEXTBITS(u4_code, u4_bitstream_offset, pu4_bitstrm_buf, 3);
 
  if(u4_code != 0)
  {
                 FLUSHBITS(u4_bitstream_offset, 3);
                 u4_run = (7 - u4_code);
  }
  else
  {
 
                 FIND_ONE_IN_STREAM_LEN(u4_code, u4_bitstream_offset,
                                        pu4_bitstrm_buf, 11);
                 u4_run = (4 + u4_code);
  }
 
             SET_BIT(ps_tu_4x4->u2_sig_coeff_map, u4_scan_pos);
  *pi2_coeff_data++ = i2_level_arr[k--];
             u4_zeroes_left -= u4_run;
             u4_scan_pos -= (u4_run + 1);
  }
 
   
   
   
         pu1_table_runbefore = (UWORD8 *)gau1_ih264d_table_run_before;
  while((u4_zeroes_left > 0) && k)
  {
             UWORD32 u4_code;
             NEXTBITS(u4_code, u4_bitstream_offset, pu4_bitstrm_buf, 3);
 
             u4_code = pu1_table_runbefore[u4_code + (u4_zeroes_left << 3)];
             u4_run = u4_code >> 2;
 
             FLUSHBITS(u4_bitstream_offset, (u4_code & 0x03));
 
             SET_BIT(ps_tu_4x4->u2_sig_coeff_map, u4_scan_pos);
  *pi2_coeff_data++ = i2_level_arr[k--];
             u4_zeroes_left -= u4_run;
             u4_scan_pos -= (u4_run + 1);
  }
   
   
   
 
   
   
   
  if(u4_zeroes_left < 0)
  return -1;
  while(k >= 0)
  {
 
             SET_BIT(ps_tu_4x4->u2_sig_coeff_map, u4_scan_pos);
  *pi2_coeff_data++ = i2_level_arr[k--];
             u4_scan_pos--;
  }
  }
 
  {
         WORD32 offset;
         offset = (UWORD8 *)pi2_coeff_data - (UWORD8 *)ps_tu_4x4;
         offset = ALIGN4(offset);
         ps_dec->pv_parse_tu_coeff_data = (void *)((UWORD8 *)ps_dec->pv_parse_tu_coeff_data + offset);
  }
 
     ps_bitstrm->u4_ofst = u4_bitstream_offset;
  return 0;
 }