void smp_set_local_oob_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

  memcpy(p_cb->sc_oob_data.loc_oob_data.private_key_used, p_cb->private_key,
         BT_OCTET32_LEN);
  p_cb->sc_oob_data.loc_oob_data.publ_key_used = p_cb->loc_publ_key;
  smp_start_nonce_generation(p_cb);
}
