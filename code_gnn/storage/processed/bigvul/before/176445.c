WORD32 ih264d_parse_buffering_period(buf_period_t *ps_buf_prd,
 dec_bit_stream_t *ps_bitstrm,
 dec_struct_t *ps_dec)
{
    UWORD8 u1_seq_parameter_set_id;
 dec_seq_params_t *ps_seq;
    UWORD8 u1_nal_hrd_present, u1_vcl_hrd_present;
    UWORD32 i;
    UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
    UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
    UNUSED(ps_buf_prd);
    u1_seq_parameter_set_id = ih264d_uev(pu4_bitstrm_ofst,
                                         pu4_bitstrm_buf);
 if(u1_seq_parameter_set_id >= MAX_NUM_SEQ_PARAMS)
 return ERROR_INVALID_SEQ_PARAM;
    ps_seq = &ps_dec->ps_sps[u1_seq_parameter_set_id];
 if(TRUE != ps_seq->u1_is_valid)
 return (-1);

    ps_dec->ps_sei->u1_seq_param_set_id = u1_seq_parameter_set_id;
    ps_dec->ps_cur_sps = ps_seq;
 if(FALSE == ps_seq->u1_is_valid)
 return ERROR_INVALID_SEQ_PARAM;
 if(1 == ps_seq->u1_vui_parameters_present_flag)
 {
        u1_nal_hrd_present = ps_seq->s_vui.u1_nal_hrd_params_present;
 if(u1_nal_hrd_present)
 {
 for(i = 0; i < ps_seq->s_vui.s_nal_hrd.u4_cpb_cnt; i++)
 {
                ih264d_get_bits_h264(
                                ps_bitstrm,
                                ps_seq->s_vui.s_nal_hrd.u1_initial_cpb_removal_delay);
                ih264d_get_bits_h264(
                                ps_bitstrm,
                                ps_seq->s_vui.s_nal_hrd.u1_initial_cpb_removal_delay);
 }
 }

        u1_vcl_hrd_present = ps_seq->s_vui.u1_vcl_hrd_params_present;
 if(u1_vcl_hrd_present)
 {
 for(i = 0; i < ps_seq->s_vui.s_vcl_hrd.u4_cpb_cnt; i++)
 {
                ih264d_get_bits_h264(
                                ps_bitstrm,
                                ps_seq->s_vui.s_vcl_hrd.u1_initial_cpb_removal_delay);
                ih264d_get_bits_h264(
                                ps_bitstrm,
                                ps_seq->s_vui.s_vcl_hrd.u1_initial_cpb_removal_delay);
 }
 }
 }
 return OK;
}
