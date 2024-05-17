IHEVCD_ERROR_T ihevcd_parse_sei(codec_t *ps_codec, nal_header_t *ps_nal)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 parse_ctxt_t *ps_parse = &ps_codec->s_parse;
    UWORD32 u4_payload_type = 0, u4_last_payload_type_byte = 0;
    UWORD32 u4_payload_size = 0, u4_last_payload_size_byte = 0;
    UWORD32 value;
 bitstrm_t *ps_bitstrm = &ps_parse->s_bitstrm;
    UWORD32 u4_bits_left;

    u4_bits_left = ihevcd_bits_num_bits_remaining(ps_bitstrm);

 while(u4_bits_left > 8)
 {
 while(ihevcd_bits_nxt(ps_bitstrm, 8) == 0xFF)
 {
            ihevcd_bits_flush(ps_bitstrm, 8);  
            u4_payload_type += 255;
 }

        BITS_PARSE("last_payload_type_byte", value, ps_bitstrm, 8);
        u4_last_payload_type_byte = value;

        u4_payload_type += u4_last_payload_type_byte;

 while(ihevcd_bits_nxt(ps_bitstrm, 8) == 0xFF)
 {
            ihevcd_bits_flush(ps_bitstrm, 8);  
            u4_payload_size += 255;
 }

        BITS_PARSE("last_payload_size_byte", value, ps_bitstrm, 8);
        u4_last_payload_size_byte = value;

        u4_payload_size += u4_last_payload_size_byte;
        u4_bits_left = ihevcd_bits_num_bits_remaining(ps_bitstrm);
        u4_payload_size = MIN(u4_payload_size, u4_bits_left / 8);
        ihevcd_parse_sei_payload(ps_codec, u4_payload_type, u4_payload_size,
                                 ps_nal->i1_nal_unit_type);

  
        u4_bits_left = ihevcd_bits_num_bits_remaining(ps_bitstrm);
 }

 if(u4_bits_left)
 {
        ihevcd_read_rbsp_trailing_bits(ps_codec, u4_bits_left);
 }

 return ret;
}
