void smp_send_init(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  smp_send_cmd(SMP_OPCODE_INIT, p_cb);
}
