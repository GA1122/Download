void smp_proc_sec_grant(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t res = *(uint8_t*)p_data;
  SMP_TRACE_DEBUG("%s", __func__);
 if (res != SMP_SUCCESS) {
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, p_data);
 } else  
 {
  
    p_cb->cb_evt = SMP_IO_CAP_REQ_EVT;
 }
}
