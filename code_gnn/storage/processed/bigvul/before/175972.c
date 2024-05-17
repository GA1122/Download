void smp_br_check_authorization_request(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t reason = SMP_SUCCESS;

  SMP_TRACE_DEBUG("%s rcvs i_keys=0x%x r_keys=0x%x (i-initiator r-responder)",
                  __func__, p_cb->local_i_key, p_cb->local_r_key);

  
  p_cb->local_i_key &= ~SMP_SEC_KEY_TYPE_LK;
  p_cb->local_r_key &= ~SMP_SEC_KEY_TYPE_LK;

  
 if (p_cb->role == HCI_ROLE_MASTER) {
    p_cb->local_r_key &= (SMP_SEC_KEY_TYPE_ID | SMP_SEC_KEY_TYPE_CSRK);
 }

  
 if ((p_cb->loc_auth_req & SMP_H7_SUPPORT_BIT) &&
 (p_cb->peer_auth_req & SMP_H7_SUPPORT_BIT)) {
    p_cb->key_derivation_h7_used = TRUE;
 }
  SMP_TRACE_DEBUG("%s: use h7 = %d", __func__, p_cb->key_derivation_h7_used);

  SMP_TRACE_DEBUG(
 "%s rcvs upgrades: i_keys=0x%x r_keys=0x%x (i-initiator r-responder)",
      __func__, p_cb->local_i_key, p_cb->local_r_key);

 if ( 
 (p_cb->local_i_key || p_cb->local_r_key)) {
    smp_br_state_machine_event(p_cb, SMP_BR_BOND_REQ_EVT, NULL);

  
 if (p_cb->role == HCI_ROLE_MASTER && p_cb->local_r_key == 0)
      smp_key_distribution_by_transport(p_cb, NULL);
 } else {
    smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &reason);
 }
}
