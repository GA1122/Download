void smp_derive_link_key_from_long_term_key(tSMP_CB* p_cb,
                                            tSMP_INT_DATA* p_data) {
  tSMP_STATUS status = SMP_PAIR_FAIL_UNKNOWN;

  SMP_TRACE_DEBUG("%s", __func__);
 if (!smp_calculate_link_key_from_long_term_key(p_cb)) {
    SMP_TRACE_ERROR("%s failed", __func__);
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = status;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 return;
 }
}
