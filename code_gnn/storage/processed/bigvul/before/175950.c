void smp_send_dhkey_check(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  smp_send_cmd(SMP_OPCODE_PAIR_DHKEY_CHECK, p_cb);
}
