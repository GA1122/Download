void smp_send_pair_public_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  smp_send_cmd(SMP_OPCODE_PAIR_PUBLIC_KEY, p_cb);
}
