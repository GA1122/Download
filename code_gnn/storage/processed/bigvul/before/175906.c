void smp_br_process_pairing_command(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = p_data->p_data;
  tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(p_cb->pairing_bda);

  SMP_TRACE_DEBUG("%s", __func__);
  
 if (!p_dev_rec->new_encryption_key_is_p256 && p_cb->role == HCI_ROLE_SLAVE) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_XTRANS_DERIVE_NOT_ALLOW;
    smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &smp_int_data);
 return;
 }

  
 if (p_dev_rec && (p_cb->role == HCI_ROLE_SLAVE))
    btm_sec_clear_ble_keys(p_dev_rec);

  p_cb->flags |= SMP_PAIR_FLAG_ENC_AFTER_PAIR;

  STREAM_TO_UINT8(p_cb->peer_io_caps, p);
  STREAM_TO_UINT8(p_cb->peer_oob_flag, p);
  STREAM_TO_UINT8(p_cb->peer_auth_req, p);
  STREAM_TO_UINT8(p_cb->peer_enc_size, p);
  STREAM_TO_UINT8(p_cb->peer_i_key, p);
  STREAM_TO_UINT8(p_cb->peer_r_key, p);

 if (smp_command_has_invalid_parameters(p_cb)) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_INVALID_PARAMETERS;
    smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &smp_int_data);
 return;
 }

  
  
  p_cb->local_i_key = p_cb->peer_i_key;
  p_cb->local_r_key = p_cb->peer_r_key;

 if (p_cb->role == HCI_ROLE_SLAVE) {
    p_dev_rec->new_encryption_key_is_p256 = false;
  
    p_cb->cb_evt = SMP_BR_KEYS_REQ_EVT;
 } else {
  
    SMP_TRACE_DEBUG(
 "%s master rcvs valid PAIRING RESPONSE."
 " Supposed to move to key distribution phase. ",
        __func__);
 }

  
  p_cb->peer_auth_req |= SMP_AUTH_BOND;
  p_cb->loc_auth_req |= SMP_AUTH_BOND;
}
