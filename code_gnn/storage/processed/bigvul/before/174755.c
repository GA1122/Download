WORD16 impeg2d_dec_vld_symbol(stream_t *ps_stream,const WORD16 ai2_code_table[][2],  UWORD16 u2_max_len)
{
  UWORD16 u2_data;
  WORD16  u2_end = 0;
  UWORD16 u2_org_max_len = u2_max_len;
  UWORD16 u2_i_bit;

  
  u2_data = impeg2d_bit_stream_nxt(ps_stream,u2_max_len);
 do
 {
    u2_max_len--;
  
    u2_i_bit = (UWORD8)((u2_data >> u2_max_len) & 0x1);

  
    u2_end = ai2_code_table[u2_end][u2_i_bit];
 }while(u2_end > 0);

  
  impeg2d_bit_stream_flush(ps_stream,(UWORD8)(u2_org_max_len - u2_max_len));
 return(u2_end);
}
