void smp_start_passkey_verification(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = NULL;

  SMP_TRACE_DEBUG("%s", __func__);
  p = p_cb->local_random;
  UINT32_TO_STREAM(p, p_data->passkey);

  p = p_cb->peer_random;
  UINT32_TO_STREAM(p, p_data->passkey);

  p_cb->round = 0;
  smp_start_nonce_generation(p_cb);
}
