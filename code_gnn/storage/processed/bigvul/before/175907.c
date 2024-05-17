void smp_br_process_security_grant(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
 if (p_data->status != SMP_SUCCESS) {
    smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, p_data);
 } else {
  
    p_cb->cb_evt = SMP_BR_KEYS_REQ_EVT;
 }
}
