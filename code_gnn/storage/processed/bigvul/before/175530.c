IHEVCD_ERROR_T ihevcd_read_rbsp_trailing_bits(codec_t *ps_codec,
                                              UWORD32 u4_bits_left)
{
 parse_ctxt_t *ps_parse = &ps_codec->s_parse;
    UWORD32 value;
    WORD32 cnt = 0;
    BITS_PARSE("rbsp_stop_one_bit", value, &ps_parse->s_bitstrm, 1);
    u4_bits_left--;
 if(value != 1)
 {
 return (IHEVCD_ERROR_T)IHEVCD_FAIL;
 }
 while(u4_bits_left)
 {
        BITS_PARSE("rbsp_alignment_zero_bit", value, &ps_parse->s_bitstrm, 1);
        u4_bits_left--;
        cnt++;
 }
    ASSERT(cnt < 8);

 return (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
}
