WORD32 ih264d_uvlc(dec_bit_stream_t *ps_bitstrm,
                   UWORD32 u4_range,
                   UWORD32 *pi_bitstrm_ofst,
                   UWORD8 u1_flag,
                   UWORD32 u4_bitstrm_ofst,
                   UWORD32 *pu4_bitstrm_buf)
{
    UWORD32 word, word2, cur_bit, cur_word, code_val, code_num, clz;

    SWITCHOFFTRACE;
    cur_bit = u4_bitstrm_ofst & 0x1F;
    cur_word = u4_bitstrm_ofst >> 5;
    word = pu4_bitstrm_buf[cur_word];
    word2 = pu4_bitstrm_buf[cur_word + 1];

 if(cur_bit != 0)
 {
        word <<= cur_bit;
        word2 >>= (32 - cur_bit);
        word |= word2;
 }

 if(u1_flag == TEV && u4_range == 1)
 {
        word >>= 31;
        word = 1 - word;
 (*pi_bitstrm_ofst)++;
        ps_bitstrm->u4_ofst = *pi_bitstrm_ofst;
 return (WORD32)word;
 }

 {
        UWORD32 ui32_code, ui32_mask;

        ui32_code = word;
        ui32_mask = 0x80000000;
        clz = 0;

  
  
 while(!(ui32_code & ui32_mask))
 {
            clz++;
            ui32_mask >>= 1;
 if(0 == ui32_mask)
 break;
 }
 }

 if(clz == 0)
 {
 *pi_bitstrm_ofst = *pi_bitstrm_ofst + (2 * clz) + 1;
        ps_bitstrm->u4_ofst = *pi_bitstrm_ofst;
 return 0;
 }

    word <<= (clz + 1);
    word >>= (32 - clz);
    code_num = (1 << clz) + word - 1;
 *pi_bitstrm_ofst = *pi_bitstrm_ofst + (2 * clz) + 1;
    ps_bitstrm->u4_ofst = *pi_bitstrm_ofst;

 if(u1_flag == TEV || u1_flag == UEV)
 return (WORD32)code_num;

    code_val = (code_num + 1) >> 1;
 if(!(code_num & 0x01))
 return -((WORD32)code_val);
 return (WORD32)code_val;

}
