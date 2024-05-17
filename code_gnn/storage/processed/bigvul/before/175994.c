void smp_proc_srk_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tBTM_LE_PCSRK_KEYS le_key;

  SMP_TRACE_DEBUG("%s", __func__);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_CSRK, true);

  
  le_key.sec_level = p_cb->sec_level;

  
  maybe_non_aligned_memcpy(le_key.csrk, p_data, BT_OCTET16_LEN);

  
  le_key.counter = 0;

 if ((p_cb->peer_auth_req & SMP_AUTH_BOND) &&
 (p_cb->loc_auth_req & SMP_AUTH_BOND))
    btm_sec_save_le_key(p_cb->pairing_bda, BTM_LE_KEY_PCSRK,
 (tBTM_LE_KEY_VALUE*)&le_key, true);
  smp_key_distribution_by_transport(p_cb, NULL);
}
