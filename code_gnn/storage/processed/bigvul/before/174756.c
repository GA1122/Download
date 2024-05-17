WORD16 impeg2d_fast_dec_vld_symbol(stream_t *ps_stream,
 const WORD16  ai2_code_table[][2],
 const UWORD16 au2_indexTable[][2],
                     UWORD16 u2_max_len)
{
    UWORD16 u2_cur_code;
    UWORD16 u2_num_bits;
    UWORD16 u2_vld_offset;
    UWORD16 u2_start_len;
    WORD16  u2_value;
    UWORD16 u2_len;
    UWORD16 u2_huffCode;

    u2_start_len  = au2_indexTable[0][0];
    u2_vld_offset = 0;
    u2_huffCode  = impeg2d_bit_stream_nxt(ps_stream,u2_max_len);
 do
 {
        u2_cur_code = u2_huffCode >> (u2_max_len - u2_start_len);
        u2_num_bits = ai2_code_table[u2_cur_code + u2_vld_offset][0];
 if(u2_num_bits == 0)
 {
            u2_huffCode  &= ((1 << (u2_max_len - u2_start_len)) - 1);
            u2_max_len    -= u2_start_len;
            u2_start_len   = au2_indexTable[ai2_code_table[u2_cur_code + u2_vld_offset][1]][0];
            u2_vld_offset  = au2_indexTable[ai2_code_table[u2_cur_code + u2_vld_offset][1]][1];
 }
 else
 {
            u2_value = ai2_code_table[u2_cur_code + u2_vld_offset][1];
            u2_len   = u2_num_bits;
 }
 }while(u2_num_bits == 0);
    impeg2d_bit_stream_flush(ps_stream,u2_len);
 return(u2_value);
}
