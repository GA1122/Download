WORD32 ih264d_parse_sei_payload(dec_bit_stream_t *ps_bitstrm,
                                UWORD32 ui4_payload_type,
                                UWORD32 ui4_payload_size,
 dec_struct_t *ps_dec)
{
    sei *ps_sei;
    WORD32 i4_status = 0;
    ps_sei = (sei *)ps_dec->ps_sei;
 switch(ui4_payload_type)
 {
 case SEI_BUF_PERIOD:

            i4_status = ih264d_parse_buffering_period(&ps_sei->s_buf_period,
                                                      ps_bitstrm, ps_dec);
  
 break;
 case SEI_PIC_TIMING:
 if(NULL == ps_dec->ps_cur_sps)
                ih264d_flush_bits_h264(ps_bitstrm, (ui4_payload_size << 3));
 else
                ih264d_parse_pic_timing(ps_bitstrm, ps_dec,
                                        ui4_payload_size);
 break;
 case SEI_RECOVERY_PT:
            ih264d_parse_recovery_point(ps_bitstrm, ps_dec,
                                        ui4_payload_size);
 break;
 default:
            ih264d_flush_bits_h264(ps_bitstrm, (ui4_payload_size << 3));
 break;
 }
 return (i4_status);
}
