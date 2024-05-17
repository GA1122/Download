WORD32 ih264d_parse_recovery_point(dec_bit_stream_t *ps_bitstrm,
 dec_struct_t *ps_dec,
                                   UWORD32 ui4_payload_size)
{
    sei *ps_sei = ps_dec->ps_sei;
 dec_err_status_t *ps_err = ps_dec->ps_dec_err_status;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UNUSED(ui4_payload_size);
    ps_sei->u2_recovery_frame_cnt = ih264d_uev(pu4_bitstrm_ofst,
                                               pu4_bitstrm_buf);
    ps_err->u4_frm_sei_sync = ps_err->u4_cur_frm
 + ps_sei->u2_recovery_frame_cnt;
    ps_sei->u1_exact_match_flag = ih264d_get_bit_h264(ps_bitstrm);
    ps_sei->u1_broken_link_flag = ih264d_get_bit_h264(ps_bitstrm);
    ps_sei->u1_changing_slice_grp_idc = ih264d_get_bits_h264(ps_bitstrm, 2);

 return (0);
}
