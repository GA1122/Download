void smp_check_auth_req(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t enc_enable = p_data->status;

  SMP_TRACE_DEBUG(
 "%s rcvs enc_enable=%d i_keys=0x%x r_keys=0x%x (i-initiator r-responder)",
      __func__, enc_enable, p_cb->local_i_key, p_cb->local_r_key);
 if (enc_enable == 1) {
 if (p_cb->le_secure_connections_mode_is_used) {
  
      p_cb->local_i_key |= SMP_SEC_KEY_TYPE_ENC;
      p_cb->local_r_key |= SMP_SEC_KEY_TYPE_ENC;

  
 if (!(p_cb->local_i_key & SMP_SEC_KEY_TYPE_LK) ||
 !(p_cb->local_r_key & SMP_SEC_KEY_TYPE_LK)) {
        p_cb->local_i_key &= ~SMP_SEC_KEY_TYPE_LK;
        p_cb->local_r_key &= ~SMP_SEC_KEY_TYPE_LK;
 }

  
 if (p_cb->role == HCI_ROLE_MASTER) {
        p_cb->local_r_key &= (SMP_SEC_KEY_TYPE_ID | SMP_SEC_KEY_TYPE_CSRK);
 }
 } else {
  
      p_cb->local_i_key &= ~SMP_SEC_KEY_TYPE_LK;
      p_cb->local_r_key &= ~SMP_SEC_KEY_TYPE_LK;
 }
    SMP_TRACE_DEBUG(
 "%s rcvs upgrades: i_keys=0x%x r_keys=0x%x (i-initiator r-responder)",
        __func__, p_cb->local_i_key, p_cb->local_r_key);

 if ( 
 (p_cb->local_i_key || p_cb->local_r_key)) {
      smp_sm_event(p_cb, SMP_BOND_REQ_EVT, NULL);
 } else {
      tSMP_INT_DATA smp_int_data;
      smp_int_data.status = enc_enable ? SMP_SUCCESS : SMP_ENC_FAIL;
      smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 }
 } else if (enc_enable == 0) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = enc_enable ? SMP_SUCCESS : SMP_ENC_FAIL;
  
 if (p_cb->flags & SMP_PAIR_FLAG_ENC_AFTER_PAIR)
      smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
  
  
 else if (p_cb->role == HCI_ROLE_MASTER) {
      smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 }
 }
}
