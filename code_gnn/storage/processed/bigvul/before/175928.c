void smp_proc_id_addr(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = p_data->p_data;
  tBTM_LE_PID_KEYS pid_key;

  SMP_TRACE_DEBUG("%s", __func__);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_ID, true);

  STREAM_TO_UINT8(pid_key.addr_type, p);
  STREAM_TO_BDADDR(pid_key.static_addr, p);
  memcpy(pid_key.irk, p_cb->tk, BT_OCTET16_LEN);

  
  p_cb->id_addr_rcvd = true;
  p_cb->id_addr_type = pid_key.addr_type;
  p_cb->id_addr = pid_key.static_addr;

  
 if ((p_cb->peer_auth_req & SMP_AUTH_BOND) &&
 (p_cb->loc_auth_req & SMP_AUTH_BOND))
    btm_sec_save_le_key(p_cb->pairing_bda, BTM_LE_KEY_PID,
 (tBTM_LE_KEY_VALUE*)&pid_key, true);
  smp_key_distribution_by_transport(p_cb, NULL);
}
