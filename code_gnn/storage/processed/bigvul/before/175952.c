void smp_send_id_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tBTM_LE_KEY_VALUE le_key;
  SMP_TRACE_DEBUG("%s", __func__);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_ID, false);

  smp_send_cmd(SMP_OPCODE_IDENTITY_INFO, p_cb);
  smp_send_cmd(SMP_OPCODE_ID_ADDR, p_cb);

 if ((p_cb->peer_auth_req & SMP_AUTH_BOND) &&
 (p_cb->loc_auth_req & SMP_AUTH_BOND))
    btm_sec_save_le_key(p_cb->pairing_bda, BTM_LE_KEY_LID, &le_key, true);

  SMP_TRACE_WARNING("%s", __func__);
  smp_key_distribution_by_transport(p_cb, NULL);
}
