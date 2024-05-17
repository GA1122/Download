void smp_pair_terminate(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  p_cb->status = SMP_CONN_TOUT;
  smp_proc_pairing_cmpl(p_cb);
}
