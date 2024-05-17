void smp_enc_cmpl(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t enc_enable = p_data->status;

  SMP_TRACE_DEBUG("%s", __func__);
  tSMP_INT_DATA smp_int_data;
  smp_int_data.status = enc_enable ? SMP_SUCCESS : SMP_ENC_FAIL;
  smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
}
