void smp_set_derive_link_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  p_cb->derive_lk = true;
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_LK, false);
  smp_key_distribution(p_cb, NULL);
}
