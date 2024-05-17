void smp_process_secure_connection_long_term_key(void) {
  tSMP_CB* p_cb = &smp_cb;

  SMP_TRACE_DEBUG("%s", __func__);
  smp_save_secure_connections_long_term_key(p_cb);

  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_ENC, false);
  smp_key_distribution(p_cb, NULL);
}
