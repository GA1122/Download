WORD32 ih264d_parse_pic_timing(dec_bit_stream_t *ps_bitstrm,
 dec_struct_t *ps_dec,
                               UWORD32 ui4_payload_size)
{
    sei *ps_sei;
 vui_t *ps_vu4;
    UWORD8 u1_cpb_dpb_present;
    UWORD8 u1_pic_struct_present_flag;
    UWORD32 u4_start_offset, u4_bits_consumed;
    UWORD8 u1_cpb_removal_delay_length, u1_dpb_output_delay_length;

    ps_sei = (sei *)ps_dec->ps_sei;
    ps_vu4 = &ps_dec->ps_cur_sps->s_vui;

    u1_cpb_dpb_present = ps_vu4->u1_vcl_hrd_params_present
 + ps_vu4->u1_nal_hrd_params_present;

 if(ps_vu4->u1_vcl_hrd_params_present)
 {
        u1_cpb_removal_delay_length =
                        ps_vu4->s_vcl_hrd.u1_cpb_removal_delay_length;
        u1_dpb_output_delay_length =
                        ps_vu4->s_vcl_hrd.u1_dpb_output_delay_length;
 }
 else if(ps_vu4->u1_nal_hrd_params_present)
 {
        u1_cpb_removal_delay_length =
                        ps_vu4->s_nal_hrd.u1_cpb_removal_delay_length;
        u1_dpb_output_delay_length =
                        ps_vu4->s_nal_hrd.u1_dpb_output_delay_length;
 }
 else
 {
        u1_cpb_removal_delay_length = 24;
        u1_dpb_output_delay_length = 24;

 }

    u4_start_offset = ps_bitstrm->u4_ofst;
 if(u1_cpb_dpb_present)
 {
        ih264d_get_bits_h264(ps_bitstrm, u1_cpb_removal_delay_length);
        ih264d_get_bits_h264(ps_bitstrm, u1_dpb_output_delay_length);
 }

    u1_pic_struct_present_flag = ps_vu4->u1_pic_struct_present_flag;
 if(u1_pic_struct_present_flag)
 {
        ps_sei->u1_pic_struct = ih264d_get_bits_h264(ps_bitstrm, 4);
        ps_dec->u1_pic_struct_copy = ps_sei->u1_pic_struct;
        ps_sei->u1_is_valid = 1;
 }
    u4_bits_consumed = ps_bitstrm->u4_ofst - u4_start_offset;
    ih264d_flush_bits_h264(ps_bitstrm,
 (ui4_payload_size << 3) - u4_bits_consumed);

 return (0);
}
