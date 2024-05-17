void smp_br_send_pair_response(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

  p_cb->local_i_key &= p_cb->peer_i_key;
  p_cb->local_r_key &= p_cb->peer_r_key;

  smp_send_cmd(SMP_OPCODE_PAIRING_RSP, p_cb);
}
