void smp_send_rand(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  smp_send_cmd(SMP_OPCODE_RAND, p_cb);
}
